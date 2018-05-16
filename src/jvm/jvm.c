#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "dlfcn.h"
#include "jvm.h"
#include "wrapper/wrapper.h"
#include "wrapper/verbose.h"

_Static_assert(sizeof(jclass) == sizeof(jobject), "We assume jclass and jobject are both same internally for the call methods");

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define container_of(ptr, type, member) ((type *)((char *)(1 ? (ptr) : &((type *)0)->member) - offsetof(type, member)))

static inline char*
ccopy(const char *str, const size_t len, const bool null_terminate)
{
   assert(str);
   char *cpy = calloc(1, len + null_terminate);
   return (cpy ? memcpy(cpy, str, len) : NULL);
}

static void
jvm_string_release(struct jvm_string *string)
{
   if (!string)
      return;

   if (string->heap)
      free((char*)string->data);

   *string = (struct jvm_string){0};
}

static bool
jvm_string_set_cstr_with_length(struct jvm_string *string, const char *data, const size_t len, const bool heap)
{
   assert(string);

   char *copy = (char*)data;
   if (heap && data && len > 0 && !(copy = ccopy(data, len, true)))
      return false;

   jvm_string_release(string);
   string->heap = heap;
   string->data = (len > 0 ? copy : NULL);
   string->size = len;
   return true;
}

static bool
jvm_string_set_cstr(struct jvm_string *string, const char *data, const bool heap)
{
   assert(string);
   return jvm_string_set_cstr_with_length(string, data, (data ? strlen(data) : 0), heap);
}

static inline bool
jvm_string_eq(const struct jvm_string *a, const struct jvm_string *b)
{
   return (a->data == b->data) || (a->size == b->size && !memcmp(a->data, b->data, a->size));
}

static void
release_array(struct jvm_object *o)
{
   assert(o);
   free(o->array.data);
}

static void
release_method(struct jvm_object *o)
{
   assert(o);
   jvm_string_release(&o->method.name);
   jvm_string_release(&o->method.signature);
}

static void
release_class(struct jvm_object *o)
{
   assert(o);
   jvm_string_release(&o->klass.name);
}

static void
release_string(struct jvm_object *o)
{
   assert(o);
   jvm_string_release(&o->string);
}

static void
jvm_object_release(struct jvm_object *o)
{
   if (!o || o->type == JVM_OBJECT_NONE)
      return;

   void (*destructor[])(struct jvm_object *o) = {
      NULL,
      NULL,
      release_array,
      release_method,
      release_class,
      release_string,
   };

   assert(o->type < JVM_OBJECT_LAST);

   if (destructor[o->type])
      destructor[o->type](o);

   *o = (struct jvm_object){0};
}

static bool
compare_opaque(const struct jvm_object *a, const struct jvm_object *b)
{
   assert(a && b);
   return (a->this_klass == b->this_klass);
}

static bool
compare_array(const struct jvm_object *a, const struct jvm_object *b)
{
   assert(a && b);
   const size_t a_sz = a->array.size * a->array.element_sz;
   const size_t b_sz = b->array.size * b->array.element_sz;
   return (a_sz == b_sz && !memcmp(a->array.data, b->array.data, a_sz));
}

static bool
compare_method(const struct jvm_object *a, const struct jvm_object *b)
{
   assert(a && b);
   return a->method.klass == b->method.klass &&
          jvm_string_eq(&a->method.name, &b->method.name) &&
          jvm_string_eq(&a->method.signature, &b->method.signature);
}

static bool
compare_class(const struct jvm_object *a, const struct jvm_object *b)
{
   assert(a && b);
   return jvm_string_eq(&a->klass.name, &b->klass.name);
}

static bool
compare_string(const struct jvm_object *a, const struct jvm_object *b)
{
   assert(a && b);
   return jvm_string_eq(&a->string, &b->string);
}

static jobject
jvm_find_object(struct jvm *jvm, const struct jvm_object *o)
{
   assert(jvm && o && o->type != JVM_OBJECT_NONE);

   bool (*comparator[])(const struct jvm_object *a, const struct jvm_object *b) = {
      NULL,
      compare_opaque,
      compare_array,
      compare_method,
      compare_class,
      compare_string,
   };

   for (uintptr_t i = 0; i < ARRAY_SIZE(jvm->objects); ++i) {
      if (o->type != jvm->objects[i].type)
         continue;

      assert(o->type < JVM_OBJECT_LAST);
      if (comparator[o->type](o, &jvm->objects[i]))
         return (jobject)(i + 1);
   }

   return NULL;
}

static jclass
jvm_make_class(struct jvm *jvm, const char *name);

static void
jvm_assign_default_class(struct jvm *jvm, struct jvm_object *o)
{
   assert(jvm && o);

   switch (o->type) {
      case JVM_OBJECT_METHOD:
         o->this_klass = jvm_make_class(jvm, "java/lang/reflect/Method");
         break;

      case JVM_OBJECT_STRING:
         o->this_klass = jvm_make_class(jvm, "java/lang/String");
         break;

      case JVM_OBJECT_NONE:
      case JVM_OBJECT_OPAQUE:
      case JVM_OBJECT_ARRAY:
      case JVM_OBJECT_CLASS:
      case JVM_OBJECT_LAST:
         // opaque objects should always have `this_class`.
         // arrays have unique classes which is handled on `jvm_new_array`
         // `jvm_make_class` points class's `this_class` to first object, which is class definition for a class
         assert(0 && "epic failure");
         break;
   }
}

static jobject
jvm_add_object(struct jvm *jvm, const struct jvm_object *o)
{
   assert(jvm && o);

   uintptr_t i;
   for (i = 0; i < ARRAY_SIZE(jvm->objects) && jvm->objects[i].type != JVM_OBJECT_NONE; ++i);
   assert(i < ARRAY_SIZE(jvm->objects) && "jvm object limit reached!");
   jvm->objects[i] = *o;

   if (!jvm->objects[i].this_klass)
      jvm_assign_default_class(jvm, &jvm->objects[i]);

   return (jobject)(i + 1);
}

static jobject
jvm_add_object_if_not_there(struct jvm *jvm, struct jvm_object *needle)
{
   assert(jvm && needle);
   jobject o;
   if ((o = jvm_find_object(jvm, needle))) {
      jvm_object_release(needle);
      return o;
   }

   return jvm_add_object(jvm, needle);
}

static struct jvm_object*
jvm_get_object(struct jvm *jvm, const jobject o)
{
   assert(jvm && (uintptr_t)o <= ARRAY_SIZE(jvm->objects));
   return (o ? &jvm->objects[(uintptr_t)o - 1] : NULL);
}

static struct jvm*
jnienv_get_jvm(JNIEnv *env)
{
   return container_of(env, struct jvm, env);
}

static struct jvm*
javavm_get_jvm(JavaVM *vm)
{
   return container_of(vm, struct jvm, vm);
}

static jint
JNIEnv_GetVersion(JNIEnv * p0)
{
   return 0;
}

static jclass
JNIEnv_DefineClass(JNIEnv* p0, const char* p1, jobject p2, const jbyte* p3, jsize p4)
{
   return NULL;
}

static jclass
jvm_make_class(struct jvm *jvm, const char *name)
{
   assert(jvm && name);
   struct jvm_object o = { .this_klass = (jclass)1, .type = JVM_OBJECT_CLASS };
   jvm_string_set_cstr(&o.klass.name, name, true);
   return jvm_add_object_if_not_there(jvm, &o);
}

static jclass
JNIEnv_FindClass(JNIEnv* p0, const char* p1)
{
   assert(p0 && p1);
   verbose("%s", p1);
   return jvm_make_class(jnienv_get_jvm(p0), p1);
}

static jmethodID
JNIEnv_FromReflectedMethod(JNIEnv* p0, jobject p1)
{
   return NULL;
}

static jfieldID
JNIEnv_FromReflectedField(JNIEnv* p0, jobject p1)
{
   return NULL;
}

static jobject
JNIEnv_ToReflectedMethod(JNIEnv* p0, jclass p1, jmethodID p2, jboolean p3)
{
   return NULL;
}

static jclass
JNIEnv_GetSuperclass(JNIEnv* p0, jclass p1)
{
   return NULL;
}

static jboolean
JNIEnv_IsAssignableFrom(JNIEnv* p0, jclass p1, jclass p2)
{
   return 0;
}

static jobject
JNIEnv_ToReflectedField(JNIEnv* p0, jclass p1, jfieldID p2, jboolean p3)
{
   return NULL;
}

static jint
JNIEnv_Throw(JNIEnv* p0, jthrowable p1)
{
   return 0;
}

static jint
JNIEnv_ThrowNew(JNIEnv * p0, jclass p1, const char * p2)
{
   return 0;
}

static jthrowable
JNIEnv_ExceptionOccurred(JNIEnv* p0)
{
   return NULL;
}

static void
JNIEnv_ExceptionDescribe(JNIEnv* p0)
{
}

static void
JNIEnv_ExceptionClear(JNIEnv* p0)
{
}

static void
JNIEnv_FatalError(JNIEnv* p0, const char* p1)
{
}

static jint
JNIEnv_PushLocalFrame(JNIEnv* p0, jint p1)
{
   return 0;
}

static jobject
JNIEnv_PopLocalFrame(JNIEnv* p0, jobject p1)
{
   return NULL;
}

static jobject
JNIEnv_NewGlobalRef(JNIEnv* p0, jobject p1)
{
   // FIXME: add ref counting
   return p1;
}

static void
JNIEnv_DeleteGlobalRef(JNIEnv* p0, jobject p1)
{
}

static void
JNIEnv_DeleteLocalRef(JNIEnv* p0, jobject p1)
{
}

static jboolean
JNIEnv_IsSameObject(JNIEnv* p0, jobject p1, jobject p2)
{
   return (p1 == p2);
}

static jobject
JNIEnv_NewLocalRef(JNIEnv* p0, jobject p1)
{
   // FIXME: add ref counting
   return NULL;
}

static jint
JNIEnv_EnsureLocalCapacity(JNIEnv* p0, jint p1)
{
   return 0;
}

static jobject
JNIEnv_AllocObject(JNIEnv* p0, jclass p1)
{
   assert(p0 && p1);
   struct jvm_object o = { .this_klass = p1, .type = JVM_OBJECT_OPAQUE };
   return jvm_add_object_if_not_there(jnienv_get_jvm(p0), &o);
}

static jobject
JNIEnv_NewObjectV(JNIEnv *p0, jclass p1, jmethodID p2, va_list p3)
{
   assert(p0 && p1);
   return JNIEnv_AllocObject(p0, p1);
   // FIXME: call constructor
}

static jobject
JNIEnv_NewObject(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   va_list ap;
   va_start(ap, p2);
   const jobject r = JNIEnv_NewObjectV(p0, p1, p2, ap);
   va_end(ap);
   return r;
}

static jobject
JNIEnv_NewObjectA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   assert(p0 && p1);
   return JNIEnv_AllocObject(p0, p1);
   // FIXME: call constructor
}

static jclass
JNIEnv_GetObjectClass(JNIEnv* env, jobject p1)
{
   assert(env && p1);
   verbose("%u", (uint32_t)(uintptr_t)p1);
   return jvm_get_object(jnienv_get_jvm(env), p1)->this_klass;
}

static jboolean
JNIEnv_IsInstanceOf(JNIEnv* p0, jobject p1, jclass p2)
{
   assert(p0 && p1 && p2);
   verbose("%u, %u", (uint32_t)(uintptr_t)p1, (uint32_t)(uintptr_t)p2);
   verbose("%s", jvm_get_object(jnienv_get_jvm(p0), jvm_get_object(jnienv_get_jvm(p0), p1)->this_klass)->klass.name.data);
   verbose("%s", jvm_get_object(jnienv_get_jvm(p0), p2)->klass.name.data);
   return jvm_get_object(jnienv_get_jvm(p0), p1)->this_klass == p2;
}

static char*
cstr_replace(char *cstr, const char replace, const char with)
{
   assert(cstr && replace != with);

   if (replace == with)
      return cstr;

   char *s = cstr;
   while ((s = strchr(s, replace)))
      *s = with;
   return cstr;
}

static void
jvm_form_symbol(struct jvm *jvm, jmethodID method_id, char *symbol, const size_t symbol_sz)
{
   assert(jvm && method_id);
   struct jvm_method *method = &jvm_get_object(jvm, method_id)->method;
   verbose("%s::%s::%s", jvm_get_object(jvm, method->klass)->klass.name.data, method->name.data, method->signature.data);
   snprintf(symbol, symbol_sz, "%s_%s", jvm_get_object(jvm, method->klass)->klass.name.data, method->name.data);
   cstr_replace(symbol, '/', '_');
   cstr_replace(symbol, '$', '_');
}

static jobject
jvm_stub_class_from_method(struct jvm *jvm, jmethodID method_id)
{
   char symbol[255];
   jvm_form_symbol(jvm, method_id, symbol, sizeof(symbol));
   return jvm_make_class(jvm, symbol);
}

static void*
jvm_wrap_method(struct jvm *jvm, jmethodID method_id)
{
   char symbol[255];
   jvm_form_symbol(jvm, method_id, symbol, sizeof(symbol));
   return wrapper_create(symbol, dlsym(RTLD_DEFAULT, symbol));
}

static void
JNIEnv_CallStaticVoidMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   assert(p0 && p1 && p2);
   union { jobject (*fun)(JNIEnv*, jclass, va_list); void *ptr; } f;
   if ((f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), p2)))
      f.fun(p0, p1, p3);
}

static void
JNIEnv_CallStaticVoidMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   va_list ap;
   va_start(ap, p2);
   JNIEnv_CallStaticVoidMethodV(p0, p1, p2, ap);
   va_end(ap);
}

static void
JNIEnv_CallStaticVoidMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   assert(p0 && p1 && p2);
   union { jobject (*fun)(JNIEnv*, jclass, jvalue*); void *ptr; } f;
   if ((f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), p2)))
      f.fun(p0, p1, p3);
}

static void
JNIEnv_CallVoidMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   JNIEnv_CallStaticVoidMethodV(p0, p1, p2, p3);
}

static void
JNIEnv_CallVoidMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   va_list ap;
   va_start(ap, p2);
   JNIEnv_CallVoidMethodV(p0, p1, p2, ap);
   va_end(ap);
}

static void
JNIEnv_CallVoidMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   JNIEnv_CallStaticVoidMethodA(p0, p1, p2, p3);
}

static void
JNIEnv_CallNonvirtualVoidMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   assert(p0 && p1 && p2 && p3);
   union { jobject (*fun)(JNIEnv*, jobject, jclass, va_list); void *ptr; } f;
   if ((f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), p2)))
      f.fun(p0, p1, p2, p4);
}

static void
JNIEnv_CallNonvirtualVoidMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   va_list ap;
   va_start(ap, p3);
   JNIEnv_CallNonvirtualVoidMethodV(p0, p1, p2, p3, ap);
   va_end(ap);
}

static void
JNIEnv_CallNonvirtualVoidMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   assert(p0 && p1 && p2 && p3);
   union { jobject (*fun)(JNIEnv*, jobject, jclass, jvalue*); void *ptr; } f;
   if ((f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), p2)))
      f.fun(p0, p1, p2, p4);
}

// N == Call method type convention (Long, Float, StaticLong, StaticFloat, etc...)
// T == C type of return value
// C == Type of second argument (jclass for static call, jobject for instance call)
// D == Default return value
#define gen_jnienv_method_call(N, T, C, D) \
   static T \
   JNIEnv_Call##N##MethodV(JNIEnv *p0, C p1, jmethodID method, va_list p3) { \
      assert(p0 && p1 && method); \
      union { T (*fun)(JNIEnv*, C, va_list); void *ptr; } f; \
      f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), method); \
      return (f.ptr ? f.fun(p0, p1, p3) : D); \
   } \
   static T \
   JNIEnv_Call##N##MethodA(JNIEnv* p0, C p1, jmethodID method, jvalue* p3) { \
      assert(p0 && p1 && method); \
      union { T (*fun)(JNIEnv*, C, jvalue*); void *ptr; } f; \
      f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), method); \
      return (f.ptr ? f.fun(p0, p1, p3) : D); \
   } \
   static T \
   JNIEnv_Call##N##Method(JNIEnv* p0, C p1, jmethodID method, ...) { \
      va_list ap; \
      va_start(ap, method); \
      const T r = JNIEnv_Call##N##MethodV(p0, p1, method, ap); \
      va_end(ap); \
      return r; \
   }

// N == Call method type name (Long, Float, etc...)
// T == C type of return value
// D == Default return value
#define gen_jnienv_nonvirtual_method_call(N, T, D) \
   static T \
   JNIEnv_CallNonvirtual##N##MethodV(JNIEnv *p0, jobject p1, jclass p2, jmethodID method, va_list p4) { \
      assert(p0 && p1 && p2 && method); \
      union { T (*fun)(JNIEnv*, jobject, jclass, va_list); void *ptr; } f; \
      f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), p2); \
      return (f.ptr ? f.fun(p0, p1, p2, p4) : D); \
   } \
   static T \
   JNIEnv_CallNonvirtual##N##MethodA(JNIEnv *p0, jobject p1, jclass p2, jmethodID method, jvalue *p4) { \
      assert(p0 && p1 && p2 && method); \
      union { T (*fun)(JNIEnv*, jobject, jclass, jvalue*); void *ptr; } f; \
      f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), p2); \
      return (f.ptr ? f.fun(p0, p1, p2, p4) : D); \
   } \
   static T \
   JNIEnv_CallNonvirtual##N##Method(JNIEnv* p0, jobject p1, jclass p2, jmethodID method, ...) { \
      va_list ap; \
      va_start(ap, method); \
      const T r = JNIEnv_CallNonvirtual##N##MethodV(p0, p1, p2, method, ap); \
      va_end(ap); \
      return r; \
   }

// N == Method type name
// T == C type of return value
// D == Default return value
#define gen_jnienv_method(N, T, D) \
   gen_jnienv_method_call(N, T, jobject, D) \
   gen_jnienv_method_call(Static##N, T, jclass, D) \
   gen_jnienv_nonvirtual_method_call(N, T, D)

gen_jnienv_method(Object, jobject, jvm_stub_class_from_method(jnienv_get_jvm(p0), method))
gen_jnienv_method(Boolean, jboolean, false)
gen_jnienv_method(Byte, jbyte, 0)
gen_jnienv_method(Char, jchar, 0)
gen_jnienv_method(Short, jshort, 0)
gen_jnienv_method(Int, jint, 0)
gen_jnienv_method(Long, jlong, 0)
gen_jnienv_method(Float, jfloat, 0)
gen_jnienv_method(Double, jdouble, 0)

// N == Property method type convention (Long, Float, StaticLong, StaticFloat, etc...)
// T == C type of return value
// D == Default return value
#define gen_jnienv_property_call(N, T, D) \
   static T \
   JNIEnv_Get##N##Field(JNIEnv *p0, jclass p1, jfieldID method) { \
      assert(p0 && p1 && method); \
      union { T (*fun)(JNIEnv*, jobject); void *ptr; } f; \
      f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), (jmethodID)method); \
      return (f.ptr ? f.fun(p0, p1) : D); \
   } \
   static void \
   JNIEnv_Set##N##Field(JNIEnv* p0, jclass p1, jfieldID method, T p3) { \
      assert(p0 && p1 && method); \
      union { void (*fun)(JNIEnv*, jobject, T); void *ptr; } f; \
      if ((f.ptr = jvm_wrap_method(jnienv_get_jvm(p0), (jmethodID)method))) \
         f.fun(p0, p1, p3); \
   }

// N == Property type name
// T == C type of return value
#define gen_jnienv_property(N, T, D) \
   gen_jnienv_property_call(N, T, D) \
   gen_jnienv_property_call(Static##N, T, D)

gen_jnienv_property(Object, jobject, jvm_stub_class_from_method(jnienv_get_jvm(p0), (jmethodID)method))
gen_jnienv_property(Boolean, jboolean, false)
gen_jnienv_property(Byte, jbyte, 0)
gen_jnienv_property(Char, jchar, 0)
gen_jnienv_property(Short, jshort, 0)
gen_jnienv_property(Int, jint, 0)
gen_jnienv_property(Long, jlong, 0)
gen_jnienv_property(Float, jfloat, 0)
gen_jnienv_property(Double, jdouble, 0)

static jmethodID
jvm_make_method(struct jvm *jvm, jclass klass, const char *name, const char *sig)
{
   assert(jvm && klass && name && sig);
   verbose("%s::%s::%s", jvm_get_object(jvm, klass)->klass.name.data, name, sig);
   struct jvm_object o = { .method.klass = klass, .type = JVM_OBJECT_METHOD };
   jvm_string_set_cstr(&o.method.name, name, true);
   jvm_string_set_cstr(&o.method.signature, sig, true);
   return jvm_add_object_if_not_there(jvm, &o);
}

static jmethodID
JNIEnv_GetMethodID(JNIEnv* p0, jclass klass, const char* name, const char* sig)
{
   return jvm_make_method(jnienv_get_jvm(p0), klass, name, sig);
}

static jmethodID
JNIEnv_GetStaticMethodID(JNIEnv* p0, jclass klass, const char* name, const char* sig)
{
   return jvm_make_method(jnienv_get_jvm(p0), klass, name, sig);
}

static jfieldID
jvm_make_fieldid(struct jvm *jvm, const jclass klass, const char *name, const char *sig)
{
   return (jfieldID)jvm_make_method(jvm, klass, name, sig);
}

static jfieldID
JNIEnv_GetFieldID(JNIEnv* p0, jclass klass, const char* name, const char* sig)
{
   return jvm_make_fieldid(jnienv_get_jvm(p0), klass, name, sig);
}

static jfieldID
JNIEnv_GetStaticFieldID(JNIEnv* p0, jclass klass, const char* name, const char* sig)
{
   return jvm_make_fieldid(jnienv_get_jvm(p0), klass, name, sig);
}

static jstring
JNIEnv_NewString(JNIEnv* p0, const jchar* p1, jsize p2)
{
   assert(p0);
   struct jvm_object o = { .type = JVM_OBJECT_STRING };
   jvm_string_set_cstr_with_length(&o.string, (const char*)p1, p2, true);
   return jvm_add_object_if_not_there(jnienv_get_jvm(p0), &o);
}

static jsize
JNIEnv_GetStringLength(JNIEnv* p0, jstring p1)
{
   return 0;
}

const jchar*
JNIEnv_GetStringChars(JNIEnv* p0, jstring p1, jboolean* p2)
{
   return NULL;
}

static void
JNIEnv_ReleaseStringChars(JNIEnv* p0, jstring p1, const jchar* p2)
{
}

static jstring
JNIEnv_NewStringUTF(JNIEnv* p0, const char* p1)
{
   assert(p0);
   verbose("%s", p1);
   struct jvm_object o = { .type = JVM_OBJECT_STRING };
   jvm_string_set_cstr(&o.string, p1, true);
   return jvm_add_object_if_not_there(jnienv_get_jvm(p0), &o);
}

static jsize
JNIEnv_GetStringUTFLength(JNIEnv* p0, jstring p1)
{
   assert(p0 && p1);
   return jvm_get_object(jnienv_get_jvm(p0), p1)->string.size;
}

static jobject
jvm_new_array(struct jvm *jvm, const size_t size, const size_t element_sz, const char *klass)
{
   assert(jvm && klass);
   struct jvm_object o = { .array = { .size = size, .element_sz = element_sz }, .type = JVM_OBJECT_ARRAY };
   o.this_klass = jvm_make_class(jvm, klass);
   o.array.data = calloc(size, element_sz);
   assert(o.array.data);
   return jvm_add_object_if_not_there(jvm, &o);
}

static jsize
JNIEnv_GetArrayLength(JNIEnv* p0, jarray p1)
{
   assert(p0 && p1);
   return jvm_get_object(jnienv_get_jvm(p0), p1)->array.size;
}

static jobjectArray
JNIEnv_NewObjectArray(JNIEnv* p0, jsize p1, jclass p2, jobject p3)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jobject), "[Ljava/lang/Object;");
}

static jbooleanArray
JNIEnv_NewBooleanArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jboolean), "[Z");
}

static jbyteArray
JNIEnv_NewByteArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jbyte), "[B");
}

static jcharArray
JNIEnv_NewCharArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jchar), "[C");
}

static jshortArray
JNIEnv_NewShortArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jshort), "[S");
}

static jintArray
JNIEnv_NewIntArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jint), "[I");
}

static jlongArray
JNIEnv_NewLongArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jlong), "[J");
}

static jfloatArray
JNIEnv_NewFloatArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jfloat), "[F");
}

static jdoubleArray
JNIEnv_NewDoubleArray(JNIEnv* p0, jsize p1)
{
   return jvm_new_array(jnienv_get_jvm(p0), p1, sizeof(jdouble), "[D");
}

static void*
jvm_get_array_elements(struct jvm *jvm, jobject array, jboolean *is_copy)
{
   assert(jvm && array);

   if (is_copy)
      *is_copy = JNI_FALSE;

   return jvm_get_object(jvm, array)->array.data;
}

static jobject
JNIEnv_GetObjectArrayElement(JNIEnv* p0, jobjectArray p1, jsize p2)
{
   const struct jvm_object *obj = jvm_get_object(jnienv_get_jvm(p0), p1);
   if (!obj || obj->array.size <= (size_t)p2)
      return NULL;

   return (jobject)((uintptr_t*)obj->array.data)[p2];
}

static void
JNIEnv_SetObjectArrayElement(JNIEnv* p0, jobjectArray p1, jsize p2, jobject p3)
{
   const struct jvm_object *obj = jvm_get_object(jnienv_get_jvm(p0), p1);
   if (!obj || obj->array.size <= (size_t)p2)
      return;

   ((uintptr_t*)obj->array.data)[p2] = (uintptr_t)p3;
}

static jboolean*
JNIEnv_GetBooleanArrayElements(JNIEnv* p0, jbooleanArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static jbyte*
JNIEnv_GetByteArrayElements(JNIEnv* p0, jbyteArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static jchar*
JNIEnv_GetCharArrayElements(JNIEnv* p0, jcharArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static jshort*
JNIEnv_GetShortArrayElements(JNIEnv* p0, jshortArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static jint*
JNIEnv_GetIntArrayElements(JNIEnv* p0, jintArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static jlong*
JNIEnv_GetLongArrayElements(JNIEnv* p0, jlongArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static jfloat*
JNIEnv_GetFloatArrayElements(JNIEnv* p0, jfloatArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static jdouble*
JNIEnv_GetDoubleArrayElements(JNIEnv* p0, jdoubleArray p1, jboolean* p2)
{
   return jvm_get_array_elements(jnienv_get_jvm(p0), p1, p2);
}

static void
JNIEnv_ReleaseBooleanArrayElements(JNIEnv* p0, jbooleanArray p1, jboolean* p2, jint p3)
{
}

static void
JNIEnv_ReleaseByteArrayElements(JNIEnv* p0, jbyteArray p1, jbyte* p2, jint p3)
{
}

static void
JNIEnv_ReleaseCharArrayElements(JNIEnv* p0, jcharArray p1, jchar* p2, jint p3)
{
}

static void
JNIEnv_ReleaseShortArrayElements(JNIEnv* p0, jshortArray p1, jshort* p2, jint p3)
{
}

static void
JNIEnv_ReleaseIntArrayElements(JNIEnv* p0, jintArray p1, jint* p2, jint p3)
{
}

static void
JNIEnv_ReleaseLongArrayElements(JNIEnv* p0, jlongArray p1, jlong* p2, jint p3)
{
}

static void
JNIEnv_ReleaseFloatArrayElements(JNIEnv* p0, jfloatArray p1, jfloat* p2, jint p3)
{
}

static void
JNIEnv_ReleaseDoubleArrayElements(JNIEnv* p0, jdoubleArray p1, jdouble* p2, jint p3)
{
}

static void
JNIEnv_GetBooleanArrayRegion(JNIEnv* p0, jbooleanArray p1, jsize p2, jsize p3, jboolean* p4)
{
}

static void
jvm_get_array_region(struct jvm *jvm, jobject obj, const size_t offset, const size_t size, void *buf)
{
   assert(jvm && obj);
   const struct jvm_array *array = &jvm_get_object(jvm, obj)->array;
   assert(offset + size <= array->size);
   memcpy(buf, (char*)array->data + offset * array->element_sz, size * array->element_sz);
}

static void
JNIEnv_GetByteArrayRegion(JNIEnv *p0, jbyteArray p1, jsize p2, jsize p3, jbyte* p4)
{
   jvm_get_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_GetCharArrayRegion(JNIEnv* p0, jcharArray p1, jsize p2, jsize p3, jchar* p4)
{
   jvm_get_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_GetShortArrayRegion(JNIEnv* p0, jshortArray p1, jsize p2, jsize p3, jshort* p4)
{
   jvm_get_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_GetIntArrayRegion(JNIEnv* p0, jintArray p1, jsize p2, jsize p3, jint* p4)
{
   jvm_get_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_GetLongArrayRegion(JNIEnv* p0, jlongArray p1, jsize p2, jsize p3, jlong* p4)
{
   jvm_get_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_GetFloatArrayRegion(JNIEnv* p0, jfloatArray p1, jsize p2, jsize p3, jfloat* p4)
{
   jvm_get_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_GetDoubleArrayRegion(JNIEnv* p0, jdoubleArray p1, jsize p2, jsize p3, jdouble* p4)
{
   jvm_get_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
jvm_set_array_region(struct jvm *jvm, jobject obj, const size_t offset, const size_t size, const void *buf)
{
   assert(jvm && obj);
   struct jvm_array *array = &jvm_get_object(jvm, obj)->array;
   assert(offset + size <= array->size);
   memcpy((char*)array->data + offset * array->element_sz, buf, size * array->element_sz);
}

static void
JNIEnv_SetBooleanArrayRegion(JNIEnv* p0, jbooleanArray p1, jsize p2, jsize p3, const jboolean* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_SetByteArrayRegion(JNIEnv* p0, jbyteArray p1, jsize p2, jsize p3, const jbyte* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_SetCharArrayRegion(JNIEnv* p0, jcharArray p1, jsize p2, jsize p3, const jchar* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_SetShortArrayRegion(JNIEnv* p0, jshortArray p1, jsize p2, jsize p3, const jshort* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_SetIntArrayRegion(JNIEnv* p0, jintArray p1, jsize p2, jsize p3, const jint* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_SetLongArrayRegion(JNIEnv* p0, jlongArray p1, jsize p2, jsize p3, const jlong* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_SetFloatArrayRegion(JNIEnv* p0, jfloatArray p1, jsize p2, jsize p3, const jfloat* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
JNIEnv_SetDoubleArrayRegion(JNIEnv* p0, jdoubleArray p1, jsize p2, jsize p3, const jdouble* p4)
{
   jvm_set_array_region(jnienv_get_jvm(p0), p1, p2, p3, p4);
}

static void
jvm_register_native_method(struct jvm *jvm, const jclass klass, const JNINativeMethod *method)
{
   assert(jvm && klass && method);
   size_t i;
   for (i = 0; i < ARRAY_SIZE(jvm->methods) && jvm->methods[i].function; ++i);
   assert(i < ARRAY_SIZE(jvm->methods) && "native method limit reached!");
   jvm->methods[i].method.klass = klass;
   jvm_string_set_cstr(&jvm->methods[i].method.name, method->name, true);
   jvm_string_set_cstr(&jvm->methods[i].method.signature, method->signature, true);
   jvm->methods[i].function = method->fnPtr;
   verbose_log("%s::%s::%s", jvm_get_object(jvm, klass)->klass.name.data, method->name, method->signature);
}

static jint
JNIEnv_RegisterNatives(JNIEnv* p0, jclass p1, const JNINativeMethod* p2, jint p3)
{
   assert(p0 && p1);
   const JNINativeMethod *method = p2;
   for (jint i = 0; i < p3; ++i, ++method)
      jvm_register_native_method(jnienv_get_jvm(p0), p1, method);
   return 0;
}

static jint
JNIEnv_UnregisterNatives(JNIEnv* p0, jclass p1)
{
   assert(p0 && p1);
   struct jvm *jvm = jnienv_get_jvm(p0);
   for (size_t i = 0; i < ARRAY_SIZE(jvm->methods) && jvm->methods[i].function; ++i) {
      if (jvm->methods[i].method.klass != p1)
         continue;
      jvm->methods[i] = (struct jvm_native_method){0};
   }
   return 0;
}

static jint
JNIEnv_MonitorEnter(JNIEnv* p0, jobject p1)
{
   return 0;
}

static jint
JNIEnv_MonitorExit(JNIEnv* p0, jobject p1)
{
   return 0;
}

static jint
JNIEnv_GetJavaVM(JNIEnv* env, JavaVM** vm)
{
   assert(env && vm);
   struct jvm *jvm = jnienv_get_jvm(env);
   *vm = (JavaVM*)&jvm->vm;
   return 0;
}

static void
JNIEnv_GetStringRegion(JNIEnv* p0, jstring p1, jsize p2, jsize p3, jchar* p4)
{
}

static void
JNIEnv_GetStringUTFRegion(JNIEnv* p0, jstring p1, jsize p2, jsize p3, char* p4)
{
}

static void*
JNIEnv_GetPrimitiveArrayCritical(JNIEnv *env, jarray array, jboolean *isCopy)
{
   return jvm_get_array_elements(jnienv_get_jvm(env), array, isCopy);
}

static void
JNIEnv_ReleasePrimitiveArrayCritical(JNIEnv *env, jarray array, void *carray, jint mode)
{
}

const jchar*
JNIEnv_GetStringCritical(JNIEnv* p0, jstring p1, jboolean* p2)
{
   return NULL;
}

static void
JNIEnv_ReleaseStringCritical(JNIEnv* p0, jstring p1, const jchar* p2)
{
}

static jweak
JNIEnv_NewWeakGlobalRef(JNIEnv* p0, jobject p1)
{
   return NULL;
}

static void
JNIEnv_DeleteWeakGlobalRef(JNIEnv* p0, jweak p1)
{
}

static jboolean
JNIEnv_ExceptionCheck(JNIEnv* p0)
{
   return 0;
}

static jobject
JNIEnv_NewDirectByteBuffer(JNIEnv* p0, void* p1, jlong p2)
{
   return NULL;
}

static void*
JNIEnv_GetDirectBufferAddress(JNIEnv* p0, jobject p1)
{
   return NULL;
}

static jlong
JNIEnv_GetDirectBufferCapacity(JNIEnv* p0, jobject p1)
{
   return 0;
}

const char*
JNIEnv_GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy)
{
   assert(env);

   if (isCopy)
      *isCopy = JNI_FALSE;

   verbose("%s", (string ? jvm_get_object(jnienv_get_jvm(env), string)->string.data : "(null)"));
   return (string ? jvm_get_object(jnienv_get_jvm(env), string)->string.data : "(null)");
}

static void
JNIEnv_ReleaseStringUTFChars(JNIEnv *env, jstring string, const char *utf)
{
   assert(env && string && utf);
}

#define WRAP(x) wrapper_create(#x, x)

static void
env_init(JNIEnv *env, struct JNINativeInterface *native)
{
   assert(env && native);
   native->GetStringUTFChars = WRAP(JNIEnv_GetStringUTFChars);
   native->ReleaseStringUTFChars = WRAP(JNIEnv_ReleaseStringUTFChars);
   native->GetVersion = WRAP(JNIEnv_GetVersion);
   native->DefineClass = WRAP(JNIEnv_DefineClass);
   native->FindClass = WRAP(JNIEnv_FindClass);
   native->FromReflectedMethod = WRAP(JNIEnv_FromReflectedMethod);
   native->FromReflectedField = WRAP(JNIEnv_FromReflectedField);
   native->ToReflectedMethod = WRAP(JNIEnv_ToReflectedMethod);
   native->GetSuperclass = WRAP(JNIEnv_GetSuperclass);
   native->IsAssignableFrom = WRAP(JNIEnv_IsAssignableFrom);
   native->ToReflectedField = WRAP(JNIEnv_ToReflectedField);
   native->Throw = WRAP(JNIEnv_Throw);
   native->ThrowNew = WRAP(JNIEnv_ThrowNew);
   native->ExceptionOccurred = WRAP(JNIEnv_ExceptionOccurred);
   native->ExceptionDescribe = WRAP(JNIEnv_ExceptionDescribe);
   native->ExceptionClear = WRAP(JNIEnv_ExceptionClear);
   native->FatalError = WRAP(JNIEnv_FatalError);
   native->PushLocalFrame = WRAP(JNIEnv_PushLocalFrame);
   native->PopLocalFrame = WRAP(JNIEnv_PopLocalFrame);
   native->NewGlobalRef = WRAP(JNIEnv_NewGlobalRef);
   native->DeleteGlobalRef = WRAP(JNIEnv_DeleteGlobalRef);
   native->DeleteLocalRef = WRAP(JNIEnv_DeleteLocalRef);
   native->IsSameObject = WRAP(JNIEnv_IsSameObject);
   native->NewLocalRef = WRAP(JNIEnv_NewLocalRef);
   native->EnsureLocalCapacity = WRAP(JNIEnv_EnsureLocalCapacity);
   native->AllocObject = WRAP(JNIEnv_AllocObject);
   native->NewObject = WRAP(JNIEnv_NewObject);
   native->NewObjectV = WRAP(JNIEnv_NewObjectV);
   native->NewObjectA = WRAP(JNIEnv_NewObjectA);
   native->GetObjectClass = WRAP(JNIEnv_GetObjectClass);
   native->IsInstanceOf = WRAP(JNIEnv_IsInstanceOf);
   native->GetMethodID = WRAP(JNIEnv_GetMethodID);
   native->CallObjectMethod = WRAP(JNIEnv_CallObjectMethod);
   native->CallObjectMethodV = WRAP(JNIEnv_CallObjectMethodV);
   native->CallObjectMethodA = WRAP(JNIEnv_CallObjectMethodA);
   native->CallBooleanMethod = WRAP(JNIEnv_CallBooleanMethod);
   native->CallBooleanMethodV = WRAP(JNIEnv_CallBooleanMethodV);
   native->CallBooleanMethodA = WRAP(JNIEnv_CallBooleanMethodA);
   native->CallByteMethod = WRAP(JNIEnv_CallByteMethod);
   native->CallByteMethodV = WRAP(JNIEnv_CallByteMethodV);
   native->CallByteMethodA = WRAP(JNIEnv_CallByteMethodA);
   native->CallCharMethod = WRAP(JNIEnv_CallCharMethod);
   native->CallCharMethodV = WRAP(JNIEnv_CallCharMethodV);
   native->CallCharMethodA = WRAP(JNIEnv_CallCharMethodA);
   native->CallShortMethod = WRAP(JNIEnv_CallShortMethod);
   native->CallShortMethodV = WRAP(JNIEnv_CallShortMethodV);
   native->CallShortMethodA = WRAP(JNIEnv_CallShortMethodA);
   native->CallIntMethod = WRAP(JNIEnv_CallIntMethod);
   native->CallIntMethodV = WRAP(JNIEnv_CallIntMethodV);
   native->CallIntMethodA = WRAP(JNIEnv_CallIntMethodA);
   native->CallLongMethod = WRAP(JNIEnv_CallLongMethod);
   native->CallLongMethodV = WRAP(JNIEnv_CallLongMethodV);
   native->CallLongMethodA = WRAP(JNIEnv_CallLongMethodA);
   native->CallFloatMethod = WRAP(JNIEnv_CallFloatMethod);
   native->CallFloatMethodV = WRAP(JNIEnv_CallFloatMethodV);
   native->CallFloatMethodA = WRAP(JNIEnv_CallFloatMethodA);
   native->CallDoubleMethod = WRAP(JNIEnv_CallDoubleMethod);
   native->CallDoubleMethodV = WRAP(JNIEnv_CallDoubleMethodV);
   native->CallDoubleMethodA = WRAP(JNIEnv_CallDoubleMethodA);
   native->CallVoidMethod = WRAP(JNIEnv_CallVoidMethod);
   native->CallVoidMethodV = WRAP(JNIEnv_CallVoidMethodV);
   native->CallVoidMethodA = WRAP(JNIEnv_CallVoidMethodA);
   native->CallNonvirtualObjectMethod = WRAP(JNIEnv_CallNonvirtualObjectMethod);
   native->CallNonvirtualObjectMethodV = WRAP(JNIEnv_CallNonvirtualObjectMethodV);
   native->CallNonvirtualObjectMethodA = WRAP(JNIEnv_CallNonvirtualObjectMethodA);
   native->CallNonvirtualBooleanMethod = WRAP(JNIEnv_CallNonvirtualBooleanMethod);
   native->CallNonvirtualBooleanMethodV = WRAP(JNIEnv_CallNonvirtualBooleanMethodV);
   native->CallNonvirtualBooleanMethodA = WRAP(JNIEnv_CallNonvirtualBooleanMethodA);
   native->CallNonvirtualByteMethod = WRAP(JNIEnv_CallNonvirtualByteMethod);
   native->CallNonvirtualByteMethodV = WRAP(JNIEnv_CallNonvirtualByteMethodV);
   native->CallNonvirtualByteMethodA = WRAP(JNIEnv_CallNonvirtualByteMethodA);
   native->CallNonvirtualCharMethod = WRAP(JNIEnv_CallNonvirtualCharMethod);
   native->CallNonvirtualCharMethodV = WRAP(JNIEnv_CallNonvirtualCharMethodV);
   native->CallNonvirtualCharMethodA = WRAP(JNIEnv_CallNonvirtualCharMethodA);
   native->CallNonvirtualShortMethod = WRAP(JNIEnv_CallNonvirtualShortMethod);
   native->CallNonvirtualShortMethodV = WRAP(JNIEnv_CallNonvirtualShortMethodV);
   native->CallNonvirtualShortMethodA = WRAP(JNIEnv_CallNonvirtualShortMethodA);
   native->CallNonvirtualIntMethod = WRAP(JNIEnv_CallNonvirtualIntMethod);
   native->CallNonvirtualIntMethodV = WRAP(JNIEnv_CallNonvirtualIntMethodV);
   native->CallNonvirtualIntMethodA = WRAP(JNIEnv_CallNonvirtualIntMethodA);
   native->CallNonvirtualLongMethod = WRAP(JNIEnv_CallNonvirtualLongMethod);
   native->CallNonvirtualLongMethodV = WRAP(JNIEnv_CallNonvirtualLongMethodV);
   native->CallNonvirtualLongMethodA = WRAP(JNIEnv_CallNonvirtualLongMethodA);
   native->CallNonvirtualFloatMethod = WRAP(JNIEnv_CallNonvirtualFloatMethod);
   native->CallNonvirtualFloatMethodV = WRAP(JNIEnv_CallNonvirtualFloatMethodV);
   native->CallNonvirtualFloatMethodA = WRAP(JNIEnv_CallNonvirtualFloatMethodA);
   native->CallNonvirtualDoubleMethod = WRAP(JNIEnv_CallNonvirtualDoubleMethod);
   native->CallNonvirtualDoubleMethodV = WRAP(JNIEnv_CallNonvirtualDoubleMethodV);
   native->CallNonvirtualDoubleMethodA = WRAP(JNIEnv_CallNonvirtualDoubleMethodA);
   native->CallNonvirtualVoidMethod = WRAP(JNIEnv_CallNonvirtualVoidMethod);
   native->CallNonvirtualVoidMethodV = WRAP(JNIEnv_CallNonvirtualVoidMethodV);
   native->CallNonvirtualVoidMethodA = WRAP(JNIEnv_CallNonvirtualVoidMethodA);
   native->GetFieldID = WRAP(JNIEnv_GetFieldID);
   native->GetObjectField = WRAP(JNIEnv_GetObjectField);
   native->GetBooleanField = WRAP(JNIEnv_GetBooleanField);
   native->GetByteField = WRAP(JNIEnv_GetByteField);
   native->GetCharField = WRAP(JNIEnv_GetCharField);
   native->GetShortField = WRAP(JNIEnv_GetShortField);
   native->GetIntField = WRAP(JNIEnv_GetIntField);
   native->GetLongField = WRAP(JNIEnv_GetLongField);
   native->GetFloatField = WRAP(JNIEnv_GetFloatField);
   native->GetDoubleField = WRAP(JNIEnv_GetDoubleField);
   native->SetObjectField = WRAP(JNIEnv_SetObjectField);
   native->SetBooleanField = WRAP(JNIEnv_SetBooleanField);
   native->SetByteField = WRAP(JNIEnv_SetByteField);
   native->SetCharField = WRAP(JNIEnv_SetCharField);
   native->SetShortField = WRAP(JNIEnv_SetShortField);
   native->SetIntField = WRAP(JNIEnv_SetIntField);
   native->SetLongField = WRAP(JNIEnv_SetLongField);
   native->SetFloatField = WRAP(JNIEnv_SetFloatField);
   native->SetDoubleField = WRAP(JNIEnv_SetDoubleField);
   native->GetStaticMethodID = WRAP(JNIEnv_GetStaticMethodID);
   native->CallStaticObjectMethod = WRAP(JNIEnv_CallStaticObjectMethod);
   native->CallStaticObjectMethodV = WRAP(JNIEnv_CallStaticObjectMethodV);
   native->CallStaticObjectMethodA = WRAP(JNIEnv_CallStaticObjectMethodA);
   native->CallStaticBooleanMethod = WRAP(JNIEnv_CallStaticBooleanMethod);
   native->CallStaticBooleanMethodV = WRAP(JNIEnv_CallStaticBooleanMethodV);
   native->CallStaticBooleanMethodA = WRAP(JNIEnv_CallStaticBooleanMethodA);
   native->CallStaticByteMethod = WRAP(JNIEnv_CallStaticByteMethod);
   native->CallStaticByteMethodV = WRAP(JNIEnv_CallStaticByteMethodV);
   native->CallStaticByteMethodA = WRAP(JNIEnv_CallStaticByteMethodA);
   native->CallStaticCharMethod = WRAP(JNIEnv_CallStaticCharMethod);
   native->CallStaticCharMethodV = WRAP(JNIEnv_CallStaticCharMethodV);
   native->CallStaticCharMethodA = WRAP(JNIEnv_CallStaticCharMethodA);
   native->CallStaticShortMethod = WRAP(JNIEnv_CallStaticShortMethod);
   native->CallStaticShortMethodV = WRAP(JNIEnv_CallStaticShortMethodV);
   native->CallStaticShortMethodA = WRAP(JNIEnv_CallStaticShortMethodA);
   native->CallStaticIntMethod = WRAP(JNIEnv_CallStaticIntMethod);
   native->CallStaticIntMethodV = WRAP(JNIEnv_CallStaticIntMethodV);
   native->CallStaticIntMethodA = WRAP(JNIEnv_CallStaticIntMethodA);
   native->CallStaticLongMethod = WRAP(JNIEnv_CallStaticLongMethod);
   native->CallStaticLongMethodV = WRAP(JNIEnv_CallStaticLongMethodV);
   native->CallStaticLongMethodA = WRAP(JNIEnv_CallStaticLongMethodA);
   native->CallStaticFloatMethod = WRAP(JNIEnv_CallStaticFloatMethod);
   native->CallStaticFloatMethodV = WRAP(JNIEnv_CallStaticFloatMethodV);
   native->CallStaticFloatMethodA = WRAP(JNIEnv_CallStaticFloatMethodA);
   native->CallStaticDoubleMethod = WRAP(JNIEnv_CallStaticDoubleMethod);
   native->CallStaticDoubleMethodV = WRAP(JNIEnv_CallStaticDoubleMethodV);
   native->CallStaticDoubleMethodA = WRAP(JNIEnv_CallStaticDoubleMethodA);
   native->CallStaticVoidMethod = WRAP(JNIEnv_CallStaticVoidMethod);
   native->CallStaticVoidMethodV = WRAP(JNIEnv_CallStaticVoidMethodV);
   native->CallStaticVoidMethodA = WRAP(JNIEnv_CallStaticVoidMethodA);
   native->GetStaticFieldID = WRAP(JNIEnv_GetStaticFieldID);
   native->GetStaticObjectField = WRAP(JNIEnv_GetStaticObjectField);
   native->GetStaticBooleanField = WRAP(JNIEnv_GetStaticBooleanField);
   native->GetStaticByteField = WRAP(JNIEnv_GetStaticByteField);
   native->GetStaticCharField = WRAP(JNIEnv_GetStaticCharField);
   native->GetStaticShortField = WRAP(JNIEnv_GetStaticShortField);
   native->GetStaticIntField = WRAP(JNIEnv_GetStaticIntField);
   native->GetStaticLongField = WRAP(JNIEnv_GetStaticLongField);
   native->GetStaticFloatField = WRAP(JNIEnv_GetStaticFloatField);
   native->GetStaticDoubleField = WRAP(JNIEnv_GetStaticDoubleField);
   native->SetStaticObjectField = WRAP(JNIEnv_SetStaticObjectField);
   native->SetStaticBooleanField = WRAP(JNIEnv_SetStaticBooleanField);
   native->SetStaticByteField = WRAP(JNIEnv_SetStaticByteField);
   native->SetStaticCharField = WRAP(JNIEnv_SetStaticCharField);
   native->SetStaticShortField = WRAP(JNIEnv_SetStaticShortField);
   native->SetStaticIntField = WRAP(JNIEnv_SetStaticIntField);
   native->SetStaticLongField = WRAP(JNIEnv_SetStaticLongField);
   native->SetStaticFloatField = WRAP(JNIEnv_SetStaticFloatField);
   native->SetStaticDoubleField = WRAP(JNIEnv_SetStaticDoubleField);
   native->NewString = WRAP(JNIEnv_NewString);
   native->GetStringLength = WRAP(JNIEnv_GetStringLength);
   native->GetStringChars = WRAP(JNIEnv_GetStringChars);
   native->ReleaseStringChars = WRAP(JNIEnv_ReleaseStringChars);
   native->NewStringUTF = WRAP(JNIEnv_NewStringUTF);
   native->GetStringUTFLength = WRAP(JNIEnv_GetStringUTFLength);
   native->GetArrayLength = WRAP(JNIEnv_GetArrayLength);
   native->NewObjectArray = WRAP(JNIEnv_NewObjectArray);
   native->GetObjectArrayElement = WRAP(JNIEnv_GetObjectArrayElement);
   native->SetObjectArrayElement = WRAP(JNIEnv_SetObjectArrayElement);
   native->NewBooleanArray = WRAP(JNIEnv_NewBooleanArray);
   native->NewByteArray = WRAP(JNIEnv_NewByteArray);
   native->NewCharArray = WRAP(JNIEnv_NewCharArray);
   native->NewShortArray = WRAP(JNIEnv_NewShortArray);
   native->NewIntArray = WRAP(JNIEnv_NewIntArray);
   native->NewLongArray = WRAP(JNIEnv_NewLongArray);
   native->NewFloatArray = WRAP(JNIEnv_NewFloatArray);
   native->NewDoubleArray = WRAP(JNIEnv_NewDoubleArray);
   native->GetBooleanArrayElements = WRAP(JNIEnv_GetBooleanArrayElements);
   native->GetByteArrayElements = WRAP(JNIEnv_GetByteArrayElements);
   native->GetCharArrayElements = WRAP(JNIEnv_GetCharArrayElements);
   native->GetShortArrayElements = WRAP(JNIEnv_GetShortArrayElements);
   native->GetIntArrayElements = WRAP(JNIEnv_GetIntArrayElements);
   native->GetLongArrayElements = WRAP(JNIEnv_GetLongArrayElements);
   native->GetFloatArrayElements = WRAP(JNIEnv_GetFloatArrayElements);
   native->GetDoubleArrayElements = WRAP(JNIEnv_GetDoubleArrayElements);
   native->ReleaseBooleanArrayElements = WRAP(JNIEnv_ReleaseBooleanArrayElements);
   native->ReleaseByteArrayElements = WRAP(JNIEnv_ReleaseByteArrayElements);
   native->ReleaseCharArrayElements = WRAP(JNIEnv_ReleaseCharArrayElements);
   native->ReleaseShortArrayElements = WRAP(JNIEnv_ReleaseShortArrayElements);
   native->ReleaseIntArrayElements = WRAP(JNIEnv_ReleaseIntArrayElements);
   native->ReleaseLongArrayElements = WRAP(JNIEnv_ReleaseLongArrayElements);
   native->ReleaseFloatArrayElements = WRAP(JNIEnv_ReleaseFloatArrayElements);
   native->ReleaseDoubleArrayElements = WRAP(JNIEnv_ReleaseDoubleArrayElements);
   native->GetBooleanArrayRegion = WRAP(JNIEnv_GetBooleanArrayRegion);
   native->GetByteArrayRegion = WRAP(JNIEnv_GetByteArrayRegion);
   native->GetCharArrayRegion = WRAP(JNIEnv_GetCharArrayRegion);
   native->GetShortArrayRegion = WRAP(JNIEnv_GetShortArrayRegion);
   native->GetIntArrayRegion = WRAP(JNIEnv_GetIntArrayRegion);
   native->GetLongArrayRegion = WRAP(JNIEnv_GetLongArrayRegion);
   native->GetFloatArrayRegion = WRAP(JNIEnv_GetFloatArrayRegion);
   native->GetDoubleArrayRegion = WRAP(JNIEnv_GetDoubleArrayRegion);
   native->SetBooleanArrayRegion = WRAP(JNIEnv_SetBooleanArrayRegion);
   native->SetByteArrayRegion = WRAP(JNIEnv_SetByteArrayRegion);
   native->SetCharArrayRegion = WRAP(JNIEnv_SetCharArrayRegion);
   native->SetShortArrayRegion = WRAP(JNIEnv_SetShortArrayRegion);
   native->SetIntArrayRegion = WRAP(JNIEnv_SetIntArrayRegion);
   native->SetLongArrayRegion = WRAP(JNIEnv_SetLongArrayRegion);
   native->SetFloatArrayRegion = WRAP(JNIEnv_SetFloatArrayRegion);
   native->SetDoubleArrayRegion = WRAP(JNIEnv_SetDoubleArrayRegion);
   native->RegisterNatives = WRAP(JNIEnv_RegisterNatives);
   native->UnregisterNatives = WRAP(JNIEnv_UnregisterNatives);
   native->MonitorEnter = WRAP(JNIEnv_MonitorEnter);
   native->MonitorExit = WRAP(JNIEnv_MonitorExit);
   native->GetJavaVM = WRAP(JNIEnv_GetJavaVM);
   native->GetStringRegion = WRAP(JNIEnv_GetStringRegion);
   native->GetStringUTFRegion = WRAP(JNIEnv_GetStringUTFRegion);
   native->GetPrimitiveArrayCritical = WRAP(JNIEnv_GetPrimitiveArrayCritical);
   native->ReleasePrimitiveArrayCritical = WRAP(JNIEnv_ReleasePrimitiveArrayCritical);
   native->GetStringCritical = WRAP(JNIEnv_GetStringCritical);
   native->ReleaseStringCritical = WRAP(JNIEnv_ReleaseStringCritical);
   native->NewWeakGlobalRef = WRAP(JNIEnv_NewWeakGlobalRef);
   native->DeleteWeakGlobalRef = WRAP(JNIEnv_DeleteWeakGlobalRef);
   native->ExceptionCheck = WRAP(JNIEnv_ExceptionCheck);
   native->NewDirectByteBuffer = WRAP(JNIEnv_NewDirectByteBuffer);
   native->GetDirectBufferAddress = WRAP(JNIEnv_GetDirectBufferAddress);
   native->GetDirectBufferCapacity = WRAP(JNIEnv_GetDirectBufferCapacity);
   *env = native;
}

static jint
JavaVM_DestroyJavaVM(JavaVM *vm)
{
   assert(vm);
   return JNI_OK;
}

static jint
JavaVM_AttachCurrentThread(JavaVM *vm, JNIEnv **env, void *args)
{
   assert(vm && env);
   *env = &javavm_get_jvm(vm)->env;
   return JNI_OK;
}

static jint
JavaVM_DetachCurrentThread(JavaVM *vm)
{
   assert(vm);
   return JNI_OK;
}

static jint
JavaVM_GetEnv(JavaVM *vm, void **env, jint version)
{
   assert(vm && env);
   *env = &javavm_get_jvm(vm)->env;
   return JNI_OK;
}

static jint
JavaVM_AttachCurrentThreadAsDaemon(JavaVM *vm, JNIEnv **env, void *args)
{
   assert(vm && env);
   *env = &javavm_get_jvm(vm)->env;
   return JNI_OK;
}

static void
vm_init(JavaVM *vm, struct JNIInvokeInterface *invoke)
{
   assert(vm && invoke);
   invoke->DestroyJavaVM = WRAP(JavaVM_DestroyJavaVM);
   invoke->AttachCurrentThread = WRAP(JavaVM_AttachCurrentThread);
   invoke->DetachCurrentThread = WRAP(JavaVM_DetachCurrentThread);
   invoke->GetEnv = WRAP(JavaVM_GetEnv);
   invoke->AttachCurrentThreadAsDaemon = WRAP(JavaVM_AttachCurrentThreadAsDaemon);
   *vm = invoke;
}

void*
jvm_get_native_method(struct jvm *jvm, const char *klass, const char *method)
{
   assert(jvm && klass && method);
   for (size_t i = 0; i < ARRAY_SIZE(jvm->methods) && jvm->methods[i].function; ++i) {
      if (!strcmp(jvm_get_object(jvm, jvm->methods[i].method.klass)->klass.name.data, klass) &&
          !strcmp(jvm->methods[i].method.name.data, method))
         return wrapper_create(method, jvm->methods[i].function);
   }
   return NULL;
}

void
jvm_release(struct jvm *jvm)
{
   if (!jvm)
      return;

   for (size_t i = 0; i < ARRAY_SIZE(jvm->objects); ++i)
      jvm_object_release(&jvm->objects[i]);

   for (size_t i = 0; i < ARRAY_SIZE(jvm->methods); ++i) {
      jvm_string_release(&jvm->methods[i].method.name);
      jvm_string_release(&jvm->methods[i].method.signature);
   }

   *jvm = (struct jvm){0};
}

void
jvm_init(struct jvm *jvm)
{
   assert(jvm);
   *jvm = (struct jvm){0};
   vm_init(&jvm->vm, &jvm->invoke);
   env_init(&jvm->env, &jvm->native);
   jvm_make_class(jvm, "java/lang/Class");
}

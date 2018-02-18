#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include "jvm.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define container_of(ptr, type, member) ((type *)((char *)(1 ? (ptr) : &((type *)0)->member) - offsetof(type, member)))

struct jstring {
   char *data;
};

struct array {
   void *data;
   long element_size;
   long length; // in elements
};

struct jclass {
   char *name;
};

struct _jmethodID {
   jclass clazz;
   char *name;
   char *sig;
};

struct jvm*
jnienv_get_jvm(JNIEnv *env)
{
   return container_of(env, struct jvm, env);
}

struct jvm*
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
JNIEnv_FindClass(JNIEnv* p0, const char* p1)
{
   assert(p0 && p1);
   printf("%s\n", p1);
   struct jclass *class = malloc(sizeof(struct jclass));
   class->name = strdup(p1);
   return class;
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
   return 0;
}

static jobject
JNIEnv_NewLocalRef(JNIEnv* p0, jobject p1)
{
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
   return NULL;
}

static jobject
JNIEnv_NewObject(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return NULL;
}

static jobject
JNIEnv_NewObjectV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jobject
JNIEnv_NewObjectA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return NULL;
}

static jclass
JNIEnv_GetObjectClass(JNIEnv* env, jobject p1)
{
   assert(p1);
   printf("%p\n", p1);
   return 0;
}

static jboolean
JNIEnv_IsInstanceOf(JNIEnv* p0, jobject p1, jclass p2)
{
   return 0;
}

static jmethodID
jnienv_make_method(jclass clazz, const char *name, const char *sig)
{
   jmethodID id = malloc(sizeof(struct _jmethodID));
   id->clazz = clazz;
   id->name = strdup(name);
   id->sig = strdup(sig);
   return id;
}

static jmethodID
JNIEnv_GetMethodID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
   printf("%s::%s\n", name, sig);
   return jnienv_make_method(clazz, name, sig);
}

static jobject
JNIEnv_CallObjectMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return NULL;
}

static jobject
JNIEnv_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
   struct _jmethodID *method = p2;
   struct jclass *class = method->clazz;
   printf("%s::%s\n", (class ? class->name : "(null)"), method->name);
   return NULL;
}

static jobject
JNIEnv_CallObjectMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return NULL;
}

static jboolean
JNIEnv_CallBooleanMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jboolean
JNIEnv_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jboolean
JNIEnv_CallBooleanMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jbyte
JNIEnv_CallByteMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jbyte
JNIEnv_CallByteMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jbyte
JNIEnv_CallByteMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jchar
JNIEnv_CallCharMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jchar
JNIEnv_CallCharMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jchar
JNIEnv_CallCharMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jshort
JNIEnv_CallShortMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jshort
JNIEnv_CallShortMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jshort
JNIEnv_CallShortMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jint
JNIEnv_CallIntMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jint
JNIEnv_CallIntMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jint
JNIEnv_CallIntMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jlong
JNIEnv_CallLongMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jlong
JNIEnv_CallLongMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jlong
JNIEnv_CallLongMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jfloat
JNIEnv_CallFloatMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jfloat
JNIEnv_CallFloatMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jfloat
JNIEnv_CallFloatMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jdouble
JNIEnv_CallDoubleMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
   return 0;
}

static jdouble
JNIEnv_CallDoubleMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jdouble
JNIEnv_CallDoubleMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static void
JNIEnv_CallVoidMethod(JNIEnv* p0, jobject p1, jmethodID p2, ...)
{
}

static void
JNIEnv_CallVoidMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
}

static void
JNIEnv_CallVoidMethodA(JNIEnv* p0, jobject p1, jmethodID p2, jvalue* p3)
{
}

static jobject
JNIEnv_CallNonvirtualObjectMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return NULL;
}

static jobject
JNIEnv_CallNonvirtualObjectMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return NULL;
}

static jobject
JNIEnv_CallNonvirtualObjectMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return NULL;
}

static jboolean
JNIEnv_CallNonvirtualBooleanMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jboolean
JNIEnv_CallNonvirtualBooleanMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jboolean
JNIEnv_CallNonvirtualBooleanMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static jbyte
JNIEnv_CallNonvirtualByteMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jbyte
JNIEnv_CallNonvirtualByteMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jbyte
JNIEnv_CallNonvirtualByteMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static jchar
JNIEnv_CallNonvirtualCharMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jchar
JNIEnv_CallNonvirtualCharMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jchar
JNIEnv_CallNonvirtualCharMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static jshort
JNIEnv_CallNonvirtualShortMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jshort
JNIEnv_CallNonvirtualShortMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jshort
JNIEnv_CallNonvirtualShortMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static jint
JNIEnv_CallNonvirtualIntMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jint
JNIEnv_CallNonvirtualIntMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jint
JNIEnv_CallNonvirtualIntMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static jlong
JNIEnv_CallNonvirtualLongMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jlong
JNIEnv_CallNonvirtualLongMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jlong
JNIEnv_CallNonvirtualLongMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static jfloat
JNIEnv_CallNonvirtualFloatMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jfloat
JNIEnv_CallNonvirtualFloatMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jfloat
JNIEnv_CallNonvirtualFloatMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static jdouble
JNIEnv_CallNonvirtualDoubleMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
   return 0;
}

static jdouble
JNIEnv_CallNonvirtualDoubleMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
   return 0;
}

static jdouble
JNIEnv_CallNonvirtualDoubleMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
   return 0;
}

static void
JNIEnv_CallNonvirtualVoidMethod(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, ...)
{
}

static void
JNIEnv_CallNonvirtualVoidMethodV(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, va_list p4)
{
}

static void
JNIEnv_CallNonvirtualVoidMethodA(JNIEnv* p0, jobject p1, jclass p2, jmethodID p3, jvalue* p4)
{
}

static jfieldID
jnienv_make_fieldid(jclass clazz, const char *name, const char *sig)
{
   return (jfieldID)jnienv_make_method(clazz,name,sig);
}

static jfieldID
JNIEnv_GetFieldID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
   struct jclass *class = (struct jclass*)clazz;
   return jnienv_make_fieldid(clazz, name, sig);
}

static jobject
JNIEnv_GetObjectField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return NULL;
}

static jboolean
JNIEnv_GetBooleanField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static jbyte
JNIEnv_GetByteField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static jchar
JNIEnv_GetCharField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static jshort
JNIEnv_GetShortField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static jint
JNIEnv_GetIntField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static jlong
JNIEnv_GetLongField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static jfloat
JNIEnv_GetFloatField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static jdouble
JNIEnv_GetDoubleField(JNIEnv* p0, jobject p1, jfieldID p2)
{
   return 0;
}

static void
JNIEnv_SetObjectField(JNIEnv* p0, jobject p1, jfieldID p2, jobject p3)
{
}

static void
JNIEnv_SetBooleanField(JNIEnv* p0, jobject p1, jfieldID p2, jboolean p3)
{
}

static void
JNIEnv_SetByteField(JNIEnv* p0, jobject p1, jfieldID p2, jbyte p3)
{
}

static void
JNIEnv_SetCharField(JNIEnv* p0, jobject p1, jfieldID p2, jchar p3)
{
}

static void
JNIEnv_SetShortField(JNIEnv* p0, jobject p1, jfieldID p2, jshort p3)
{
}

static void
JNIEnv_SetIntField(JNIEnv* p0, jobject p1, jfieldID p2, jint p3)
{
}

static void
JNIEnv_SetLongField(JNIEnv* p0, jobject p1, jfieldID p2, jlong p3)
{
}

static void
JNIEnv_SetFloatField(JNIEnv* p0, jobject p1, jfieldID p2, jfloat p3)
{
}

static void
JNIEnv_SetDoubleField(JNIEnv* p0, jobject p1, jfieldID p2, jdouble p3)
{
}

static jmethodID
JNIEnv_GetStaticMethodID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
   printf("%s::%s\n", name, sig);
   return jnienv_make_method(clazz, name, sig);
}

static jobject
JNIEnv_CallStaticObjectMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return NULL;
}

static jobject
JNIEnv_CallStaticObjectMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   struct _jmethodID *method = p2;
   struct jclass *class = method->clazz;
   printf("%s::%s\n", (class ? class->name : "(null)"), method->name);
   return NULL;
}

static jobject
JNIEnv_CallStaticObjectMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return NULL;
}

static jboolean
JNIEnv_CallStaticBooleanMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jboolean
JNIEnv_CallStaticBooleanMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jboolean
JNIEnv_CallStaticBooleanMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jbyte
JNIEnv_CallStaticByteMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jbyte
JNIEnv_CallStaticByteMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jbyte
JNIEnv_CallStaticByteMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jchar
JNIEnv_CallStaticCharMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jchar
JNIEnv_CallStaticCharMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jchar
JNIEnv_CallStaticCharMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jshort
JNIEnv_CallStaticShortMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jshort
JNIEnv_CallStaticShortMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jshort
JNIEnv_CallStaticShortMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jint
JNIEnv_CallStaticIntMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jint
JNIEnv_CallStaticIntMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jint
JNIEnv_CallStaticIntMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jlong
JNIEnv_CallStaticLongMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jlong
JNIEnv_CallStaticLongMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jlong
JNIEnv_CallStaticLongMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jfloat
JNIEnv_CallStaticFloatMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jfloat
JNIEnv_CallStaticFloatMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jfloat
JNIEnv_CallStaticFloatMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static jdouble
JNIEnv_CallStaticDoubleMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
   return 0;
}

static jdouble
JNIEnv_CallStaticDoubleMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
   return 0;
}

static jdouble
JNIEnv_CallStaticDoubleMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
   return 0;
}

static void
JNIEnv_CallStaticVoidMethod(JNIEnv* p0, jclass p1, jmethodID p2, ...)
{
}

static void
JNIEnv_CallStaticVoidMethodV(JNIEnv* p0, jclass p1, jmethodID p2, va_list p3)
{
}

static void
JNIEnv_CallStaticVoidMethodA(JNIEnv* p0, jclass p1, jmethodID p2, jvalue* p3)
{
}

static jfieldID
JNIEnv_GetStaticFieldID(JNIEnv* p0, jclass clazz, const char* name, const char* sig)
{
   struct jclass *class = (struct jclass*)clazz;
   return jnienv_make_fieldid(clazz, name, sig);
}

static jobject
JNIEnv_GetStaticObjectField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return NULL;
}

static jboolean
JNIEnv_GetStaticBooleanField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static jbyte
JNIEnv_GetStaticByteField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static jchar
JNIEnv_GetStaticCharField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static jshort
JNIEnv_GetStaticShortField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static jint
JNIEnv_GetStaticIntField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static jlong
JNIEnv_GetStaticLongField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static jfloat
JNIEnv_GetStaticFloatField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static jdouble
JNIEnv_GetStaticDoubleField(JNIEnv* p0, jclass p1, jfieldID p2)
{
   return 0;
}

static void
JNIEnv_SetStaticObjectField(JNIEnv* p0, jclass p1, jfieldID p2, jobject p3)
{
}

static void
JNIEnv_SetStaticBooleanField(JNIEnv* p0, jclass p1, jfieldID p2, jboolean p3)
{
}

static void
JNIEnv_SetStaticByteField(JNIEnv* p0, jclass p1, jfieldID p2, jbyte p3)
{
}

static void
JNIEnv_SetStaticCharField(JNIEnv* p0, jclass p1, jfieldID p2, jchar p3)
{
}

static void
JNIEnv_SetStaticShortField(JNIEnv* p0, jclass p1, jfieldID p2, jshort p3)
{
}

static void
JNIEnv_SetStaticIntField(JNIEnv* p0, jclass p1, jfieldID p2, jint p3)
{
}

static void
JNIEnv_SetStaticLongField(JNIEnv* p0, jclass p1, jfieldID p2, jlong p3)
{
}

static void
JNIEnv_SetStaticFloatField(JNIEnv* p0, jclass p1, jfieldID p2, jfloat p3)
{
}

static void
JNIEnv_SetStaticDoubleField(JNIEnv* p0, jclass p1, jfieldID p2, jdouble p3)
{
}

static jstring
JNIEnv_NewString(JNIEnv* p0, const jchar* p1, jsize p2)
{
   char *str = calloc(1, p2 + 1);
   memcpy(str, p1, p2);
   struct jstring *result = malloc(sizeof(struct jstring));
   result->data = str;
   return result;
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
   struct jstring *result = malloc(sizeof(struct jstring));
   result->data = strdup((p1 ? p1 : "(null)"));
   return result;
}

static jsize
JNIEnv_GetStringUTFLength(JNIEnv* p0, jstring p1)
{
   struct jstring *result = p1;
   return strlen(result->data);
}

static jsize
JNIEnv_GetArrayLength(JNIEnv* env, jarray p1)
{
   return (p1 ? ((struct array*)p1)->length : 0);
}

static jobjectArray
JNIEnv_NewObjectArray(JNIEnv* p0, jsize p1, jclass p2, jobject p3)
{
   return NULL;
}

static jobject
JNIEnv_GetObjectArrayElement(JNIEnv* p0, jobjectArray p1, jsize p2)
{
   return NULL;
}

static void
JNIEnv_SetObjectArrayElement(JNIEnv* p0, jobjectArray p1, jsize p2, jobject p3)
{
}

static void *
new_array(jsize len, jsize element_size)
{
   if (len <= 0)
      return NULL;

   struct array *array;
   if (!(array = malloc(sizeof(*array))))
      return NULL;

   if (!(array->data = calloc(len, element_size))) {
      free(array);
      return NULL;
   }

   array->element_size = element_size;
   array->length = len;
   return array;
}

static jbooleanArray
JNIEnv_NewBooleanArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jboolean));
}


static jbyteArray
JNIEnv_NewByteArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jbyte));
}

static jcharArray
JNIEnv_NewCharArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jchar));
}

static jshortArray
JNIEnv_NewShortArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jshort));
}

static jintArray
JNIEnv_NewIntArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jint));
}

static jlongArray
JNIEnv_NewLongArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jlong));
}

static jfloatArray
JNIEnv_NewFloatArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jfloat));
}

static jdoubleArray
JNIEnv_NewDoubleArray(JNIEnv* p0, jsize p1)
{
   return new_array(p1, sizeof(jdouble));
}

static void*
get_array_elements(JNIEnv *env, void *array, jboolean *isCopy)
{
   if (isCopy)
      *isCopy = JNI_FALSE;

   return (array ? ((struct array*)array)->data : NULL);
}

static jboolean*
JNIEnv_GetBooleanArrayElements(JNIEnv* p0, jbooleanArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
}

static jbyte*
JNIEnv_GetByteArrayElements(JNIEnv* p0, jbyteArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
}

static jchar*
JNIEnv_GetCharArrayElements(JNIEnv* p0, jcharArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
}

static jshort*
JNIEnv_GetShortArrayElements(JNIEnv* p0, jshortArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
}

static jint*
JNIEnv_GetIntArrayElements(JNIEnv* p0, jintArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
}

static jlong*
JNIEnv_GetLongArrayElements(JNIEnv* p0, jlongArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
}

static jfloat*
JNIEnv_GetFloatArrayElements(JNIEnv* p0, jfloatArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
}

static jdouble*
JNIEnv_GetDoubleArrayElements(JNIEnv* p0, jdoubleArray p1, jboolean* p2)
{
   return get_array_elements(p0, p1, p2);
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
get_array_region(JNIEnv *env, const void *arrayobj, jsize start, jsize len, void *buf)
{
   if (!arrayobj)
      return;

   const struct array *array = arrayobj;
   assert(start + len <= array->length);
   start *= array->element_size;
   len *= array->element_size;
   memcpy(buf, (char *)array->data + start, len);
}

static void
JNIEnv_GetByteArrayRegion(JNIEnv *env, jbyteArray arrayobj, jsize start, jsize len, jbyte* buf)
{

   get_array_region(env, arrayobj, start, len, buf);
}

static void
JNIEnv_GetCharArrayRegion(JNIEnv* p0, jcharArray p1, jsize p2, jsize p3, jchar* p4)
{

   get_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_GetShortArrayRegion(JNIEnv* p0, jshortArray p1, jsize p2, jsize p3, jshort* p4)
{

   get_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_GetIntArrayRegion(JNIEnv* p0, jintArray p1, jsize p2, jsize p3, jint* p4)
{

   get_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_GetLongArrayRegion(JNIEnv* p0, jlongArray p1, jsize p2, jsize p3, jlong* p4)
{

   get_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_GetFloatArrayRegion(JNIEnv* p0, jfloatArray p1, jsize p2, jsize p3, jfloat* p4)
{

   get_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_GetDoubleArrayRegion(JNIEnv* p0, jdoubleArray p1, jsize p2, jsize p3, jdouble* p4)
{

   get_array_region(p0, p1, p2, p3, p4);
}

static void
set_array_region(JNIEnv *env, void *arrayobj, jsize start, jsize len, const void *buf)
{
   if (!arrayobj)
      return;

   const struct array *array = arrayobj;
   assert(start + len <= array->length);
   start *= array->element_size;
   len *= array->element_size;
   memcpy((char *)array->data + start, buf, len);
}

static void
JNIEnv_SetBooleanArrayRegion(JNIEnv* p0, jbooleanArray p1, jsize p2, jsize p3, const jboolean* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_SetByteArrayRegion(JNIEnv* p0, jbyteArray p1, jsize p2, jsize p3, const jbyte* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_SetCharArrayRegion(JNIEnv* p0, jcharArray p1, jsize p2, jsize p3, const jchar* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}


static void
JNIEnv_SetShortArrayRegion(JNIEnv* p0, jshortArray p1, jsize p2, jsize p3, const jshort* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}

static void
JNIEnv_SetIntArrayRegion(JNIEnv* p0, jintArray p1, jsize p2, jsize p3, const jint* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}


static void
JNIEnv_SetLongArrayRegion(JNIEnv* p0, jlongArray p1, jsize p2, jsize p3, const jlong* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}


static void
JNIEnv_SetFloatArrayRegion(JNIEnv* p0, jfloatArray p1, jsize p2, jsize p3, const jfloat* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}


static void
JNIEnv_SetDoubleArrayRegion(JNIEnv* p0, jdoubleArray p1, jsize p2, jsize p3, const jdouble* p4)
{
   set_array_region(p0, p1, p2, p3, p4);
}

static void
jnienv_register_jvm_native_method(JNIEnv *env, const char *klass, const char *method, void *function)
{
   size_t i;
   struct jvm *jvm = jnienv_get_jvm(env);
   for (i = 0; i < ARRAY_SIZE(jvm->methods) && jvm->methods[i].function; ++i);
   assert(i < ARRAY_SIZE(jvm->methods) && "native method limit reached!");
   jvm->methods[i].klass = strdup(klass);
   jvm->methods[i].method = strdup(method);
   jvm->methods[i].function = function;
   printf("%s::%s\n", klass, method);
}

static jint
JNIEnv_RegisterNatives(JNIEnv* p0, jclass p1, const JNINativeMethod* p2, jint p3)
{
   assert(p0 && p1);
   const struct jclass *clazz = (struct jclass*)p1;
   const JNINativeMethod *method = p2;
   for (int i = 0; i < p3; ++i, ++method)
      jnienv_register_jvm_native_method(p0, clazz->name, method->name, method->fnPtr);
   return 0;
}

static jint
JNIEnv_UnregisterNatives(JNIEnv* p0, jclass p1)
{
   assert(p0 && p1);
   const struct jclass *klass = (struct jclass*)p1;
   struct jvm *jvm = jnienv_get_jvm(p0);
   for (size_t i = 0; i < ARRAY_SIZE(jvm->methods) && jvm->methods[i].function; ++i) {
      if (strcmp(jvm->methods[i].klass, klass->name))
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
   return get_array_elements(env, array, isCopy);
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

   if (!string)
      return strdup("");

   struct jstring *str = (struct jstring*)string;
   return str->data;
}

static void
JNIEnv_ReleaseStringUTFChars(JNIEnv *env, jstring string, const char *utf)
{
   assert(env && string);
   free((void*)utf);
}

static void
trace(const char *const symbol)
{
    printf("trace: %s\n", symbol);
}

extern void* create_wrapper(const char *const symbol, void *function);
#define WRAP(x) create_wrapper(#x, x)

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

void
jvm_init(struct jvm *jvm)
{
   assert(jvm);
   *jvm = (struct jvm){0};
   vm_init(&jvm->vm, &jvm->invoke);
   env_init(&jvm->env, &jvm->native);
}

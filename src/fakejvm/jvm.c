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
}

static jint
JNIEnv_RegisterNatives(JNIEnv* p0, jclass p1, const JNINativeMethod* p2, jint p3)
{
   const struct jclass *clazz = (struct jclass*)p1;
   const JNINativeMethod *method = p2;
   for (int i = 0; i < p3; ++i, ++method) {
      jnienv_register_jvm_native_method(p0, clazz->name, method->name, method->fnPtr);
   }
   return 0;
}

static jint
JNIEnv_UnregisterNatives(JNIEnv* p0, jclass p1)
{
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
   if (!string)
      return strdup("");

   struct jstring *str = (struct jstring*)string;

   return str->data;
}

static void
JNIEnv_ReleaseStringUTFChars(JNIEnv *env, jstring string, const char *utf)
{
   free((void*)utf);
}

static void
env_init(JNIEnv *env, struct JNINativeInterface *native)
{
   assert(env && native);
   native->GetStringUTFChars = JNIEnv_GetStringUTFChars;
   native->ReleaseStringUTFChars = JNIEnv_ReleaseStringUTFChars;
   native->GetVersion = JNIEnv_GetVersion;
   native->DefineClass = JNIEnv_DefineClass;
   native->FindClass = JNIEnv_FindClass;
   native->FromReflectedMethod = JNIEnv_FromReflectedMethod;
   native->FromReflectedField = JNIEnv_FromReflectedField;
   native->ToReflectedMethod = JNIEnv_ToReflectedMethod;
   native->GetSuperclass = JNIEnv_GetSuperclass;
   native->IsAssignableFrom = JNIEnv_IsAssignableFrom;
   native->ToReflectedField = JNIEnv_ToReflectedField;
   native->Throw = JNIEnv_Throw;
   native->ThrowNew = JNIEnv_ThrowNew;
   native->ExceptionOccurred = JNIEnv_ExceptionOccurred;
   native->ExceptionDescribe = JNIEnv_ExceptionDescribe;
   native->ExceptionClear = JNIEnv_ExceptionClear;
   native->FatalError = JNIEnv_FatalError;
   native->PushLocalFrame = JNIEnv_PushLocalFrame;
   native->PopLocalFrame = JNIEnv_PopLocalFrame;
   native->NewGlobalRef = JNIEnv_NewGlobalRef;
   native->DeleteGlobalRef = JNIEnv_DeleteGlobalRef;
   native->DeleteLocalRef = JNIEnv_DeleteLocalRef;
   native->IsSameObject = JNIEnv_IsSameObject;
   native->NewLocalRef = JNIEnv_NewLocalRef;
   native->EnsureLocalCapacity = JNIEnv_EnsureLocalCapacity;
   native->AllocObject = JNIEnv_AllocObject;
   native->NewObject = JNIEnv_NewObject;
   native->NewObjectV = JNIEnv_NewObjectV;
   native->NewObjectA = JNIEnv_NewObjectA;
   native->GetObjectClass = JNIEnv_GetObjectClass;
   native->IsInstanceOf = JNIEnv_IsInstanceOf;
   native->GetMethodID = JNIEnv_GetMethodID;
   native->CallObjectMethod = JNIEnv_CallObjectMethod;
   native->CallObjectMethodV = JNIEnv_CallObjectMethodV;
   native->CallObjectMethodA = JNIEnv_CallObjectMethodA;
   native->CallBooleanMethod = JNIEnv_CallBooleanMethod;
   native->CallBooleanMethodV = JNIEnv_CallBooleanMethodV;
   native->CallBooleanMethodA = JNIEnv_CallBooleanMethodA;
   native->CallByteMethod = JNIEnv_CallByteMethod;
   native->CallByteMethodV = JNIEnv_CallByteMethodV;
   native->CallByteMethodA = JNIEnv_CallByteMethodA;
   native->CallCharMethod = JNIEnv_CallCharMethod;
   native->CallCharMethodV = JNIEnv_CallCharMethodV;
   native->CallCharMethodA = JNIEnv_CallCharMethodA;
   native->CallShortMethod = JNIEnv_CallShortMethod;
   native->CallShortMethodV = JNIEnv_CallShortMethodV;
   native->CallShortMethodA = JNIEnv_CallShortMethodA;
   native->CallIntMethod = JNIEnv_CallIntMethod;
   native->CallIntMethodV = JNIEnv_CallIntMethodV;
   native->CallIntMethodA = JNIEnv_CallIntMethodA;
   native->CallLongMethod = JNIEnv_CallLongMethod;
   native->CallLongMethodV = JNIEnv_CallLongMethodV;
   native->CallLongMethodA = JNIEnv_CallLongMethodA;
   native->CallFloatMethod = JNIEnv_CallFloatMethod;
   native->CallFloatMethodV = JNIEnv_CallFloatMethodV;
   native->CallFloatMethodA = JNIEnv_CallFloatMethodA;
   native->CallDoubleMethod = JNIEnv_CallDoubleMethod;
   native->CallDoubleMethodV = JNIEnv_CallDoubleMethodV;
   native->CallDoubleMethodA = JNIEnv_CallDoubleMethodA;
   native->CallVoidMethod = JNIEnv_CallVoidMethod;
   native->CallVoidMethodV = JNIEnv_CallVoidMethodV;
   native->CallVoidMethodA = JNIEnv_CallVoidMethodA;
   native->CallNonvirtualObjectMethod = JNIEnv_CallNonvirtualObjectMethod;
   native->CallNonvirtualObjectMethodV = JNIEnv_CallNonvirtualObjectMethodV;
   native->CallNonvirtualObjectMethodA = JNIEnv_CallNonvirtualObjectMethodA;
   native->CallNonvirtualBooleanMethod = JNIEnv_CallNonvirtualBooleanMethod;
   native->CallNonvirtualBooleanMethodV = JNIEnv_CallNonvirtualBooleanMethodV;
   native->CallNonvirtualBooleanMethodA = JNIEnv_CallNonvirtualBooleanMethodA;
   native->CallNonvirtualByteMethod = JNIEnv_CallNonvirtualByteMethod;
   native->CallNonvirtualByteMethodV = JNIEnv_CallNonvirtualByteMethodV;
   native->CallNonvirtualByteMethodA = JNIEnv_CallNonvirtualByteMethodA;
   native->CallNonvirtualCharMethod = JNIEnv_CallNonvirtualCharMethod;
   native->CallNonvirtualCharMethodV = JNIEnv_CallNonvirtualCharMethodV;
   native->CallNonvirtualCharMethodA = JNIEnv_CallNonvirtualCharMethodA;
   native->CallNonvirtualShortMethod = JNIEnv_CallNonvirtualShortMethod;
   native->CallNonvirtualShortMethodV = JNIEnv_CallNonvirtualShortMethodV;
   native->CallNonvirtualShortMethodA = JNIEnv_CallNonvirtualShortMethodA;
   native->CallNonvirtualIntMethod = JNIEnv_CallNonvirtualIntMethod;
   native->CallNonvirtualIntMethodV = JNIEnv_CallNonvirtualIntMethodV;
   native->CallNonvirtualIntMethodA = JNIEnv_CallNonvirtualIntMethodA;
   native->CallNonvirtualLongMethod = JNIEnv_CallNonvirtualLongMethod;
   native->CallNonvirtualLongMethodV = JNIEnv_CallNonvirtualLongMethodV;
   native->CallNonvirtualLongMethodA = JNIEnv_CallNonvirtualLongMethodA;
   native->CallNonvirtualFloatMethod = JNIEnv_CallNonvirtualFloatMethod;
   native->CallNonvirtualFloatMethodV = JNIEnv_CallNonvirtualFloatMethodV;
   native->CallNonvirtualFloatMethodA = JNIEnv_CallNonvirtualFloatMethodA;
   native->CallNonvirtualDoubleMethod = JNIEnv_CallNonvirtualDoubleMethod;
   native->CallNonvirtualDoubleMethodV = JNIEnv_CallNonvirtualDoubleMethodV;
   native->CallNonvirtualDoubleMethodA = JNIEnv_CallNonvirtualDoubleMethodA;
   native->CallNonvirtualVoidMethod = JNIEnv_CallNonvirtualVoidMethod;
   native->CallNonvirtualVoidMethodV = JNIEnv_CallNonvirtualVoidMethodV;
   native->CallNonvirtualVoidMethodA = JNIEnv_CallNonvirtualVoidMethodA;
   native->GetFieldID = JNIEnv_GetFieldID;
   native->GetObjectField = JNIEnv_GetObjectField;
   native->GetBooleanField = JNIEnv_GetBooleanField;
   native->GetByteField = JNIEnv_GetByteField;
   native->GetCharField = JNIEnv_GetCharField;
   native->GetShortField = JNIEnv_GetShortField;
   native->GetIntField = JNIEnv_GetIntField;
   native->GetLongField = JNIEnv_GetLongField;
   native->GetFloatField = JNIEnv_GetFloatField;
   native->GetDoubleField = JNIEnv_GetDoubleField;
   native->SetObjectField = JNIEnv_SetObjectField;
   native->SetBooleanField = JNIEnv_SetBooleanField;
   native->SetByteField = JNIEnv_SetByteField;
   native->SetCharField = JNIEnv_SetCharField;
   native->SetShortField = JNIEnv_SetShortField;
   native->SetIntField = JNIEnv_SetIntField;
   native->SetLongField = JNIEnv_SetLongField;
   native->SetFloatField = JNIEnv_SetFloatField;
   native->SetDoubleField = JNIEnv_SetDoubleField;
   native->GetStaticMethodID = JNIEnv_GetStaticMethodID;
   native->CallStaticObjectMethod = JNIEnv_CallStaticObjectMethod;
   native->CallStaticObjectMethodV = JNIEnv_CallStaticObjectMethodV;
   native->CallStaticObjectMethodA = JNIEnv_CallStaticObjectMethodA;
   native->CallStaticBooleanMethod = JNIEnv_CallStaticBooleanMethod;
   native->CallStaticBooleanMethodV = JNIEnv_CallStaticBooleanMethodV;
   native->CallStaticBooleanMethodA = JNIEnv_CallStaticBooleanMethodA;
   native->CallStaticByteMethod = JNIEnv_CallStaticByteMethod;
   native->CallStaticByteMethodV = JNIEnv_CallStaticByteMethodV;
   native->CallStaticByteMethodA = JNIEnv_CallStaticByteMethodA;
   native->CallStaticCharMethod = JNIEnv_CallStaticCharMethod;
   native->CallStaticCharMethodV = JNIEnv_CallStaticCharMethodV;
   native->CallStaticCharMethodA = JNIEnv_CallStaticCharMethodA;
   native->CallStaticShortMethod = JNIEnv_CallStaticShortMethod;
   native->CallStaticShortMethodV = JNIEnv_CallStaticShortMethodV;
   native->CallStaticShortMethodA = JNIEnv_CallStaticShortMethodA;
   native->CallStaticIntMethod = JNIEnv_CallStaticIntMethod;
   native->CallStaticIntMethodV = JNIEnv_CallStaticIntMethodV;
   native->CallStaticIntMethodA = JNIEnv_CallStaticIntMethodA;
   native->CallStaticLongMethod = JNIEnv_CallStaticLongMethod;
   native->CallStaticLongMethodV = JNIEnv_CallStaticLongMethodV;
   native->CallStaticLongMethodA = JNIEnv_CallStaticLongMethodA;
   native->CallStaticFloatMethod = JNIEnv_CallStaticFloatMethod;
   native->CallStaticFloatMethodV = JNIEnv_CallStaticFloatMethodV;
   native->CallStaticFloatMethodA = JNIEnv_CallStaticFloatMethodA;
   native->CallStaticDoubleMethod = JNIEnv_CallStaticDoubleMethod;
   native->CallStaticDoubleMethodV = JNIEnv_CallStaticDoubleMethodV;
   native->CallStaticDoubleMethodA = JNIEnv_CallStaticDoubleMethodA;
   native->CallStaticVoidMethod = JNIEnv_CallStaticVoidMethod;
   native->CallStaticVoidMethodV = JNIEnv_CallStaticVoidMethodV;
   native->CallStaticVoidMethodA = JNIEnv_CallStaticVoidMethodA;
   native->GetStaticFieldID = JNIEnv_GetStaticFieldID;
   native->GetStaticObjectField = JNIEnv_GetStaticObjectField;
   native->GetStaticBooleanField = JNIEnv_GetStaticBooleanField;
   native->GetStaticByteField = JNIEnv_GetStaticByteField;
   native->GetStaticCharField = JNIEnv_GetStaticCharField;
   native->GetStaticShortField = JNIEnv_GetStaticShortField;
   native->GetStaticIntField = JNIEnv_GetStaticIntField;
   native->GetStaticLongField = JNIEnv_GetStaticLongField;
   native->GetStaticFloatField = JNIEnv_GetStaticFloatField;
   native->GetStaticDoubleField = JNIEnv_GetStaticDoubleField;
   native->SetStaticObjectField = JNIEnv_SetStaticObjectField;
   native->SetStaticBooleanField = JNIEnv_SetStaticBooleanField;
   native->SetStaticByteField = JNIEnv_SetStaticByteField;
   native->SetStaticCharField = JNIEnv_SetStaticCharField;
   native->SetStaticShortField = JNIEnv_SetStaticShortField;
   native->SetStaticIntField = JNIEnv_SetStaticIntField;
   native->SetStaticLongField = JNIEnv_SetStaticLongField;
   native->SetStaticFloatField = JNIEnv_SetStaticFloatField;
   native->SetStaticDoubleField = JNIEnv_SetStaticDoubleField;
   native->NewString = JNIEnv_NewString;
   native->GetStringLength = JNIEnv_GetStringLength;
   native->GetStringChars = JNIEnv_GetStringChars;
   native->ReleaseStringChars = JNIEnv_ReleaseStringChars;
   native->NewStringUTF = JNIEnv_NewStringUTF;
   native->GetStringUTFLength = JNIEnv_GetStringUTFLength;
   native->GetArrayLength = JNIEnv_GetArrayLength;
   native->NewObjectArray = JNIEnv_NewObjectArray;
   native->GetObjectArrayElement = JNIEnv_GetObjectArrayElement;
   native->SetObjectArrayElement = JNIEnv_SetObjectArrayElement;
   native->NewBooleanArray = JNIEnv_NewBooleanArray;
   native->NewByteArray = JNIEnv_NewByteArray;
   native->NewCharArray = JNIEnv_NewCharArray;
   native->NewShortArray = JNIEnv_NewShortArray;
   native->NewIntArray = JNIEnv_NewIntArray;
   native->NewLongArray = JNIEnv_NewLongArray;
   native->NewFloatArray = JNIEnv_NewFloatArray;
   native->NewDoubleArray = JNIEnv_NewDoubleArray;
   native->GetBooleanArrayElements = JNIEnv_GetBooleanArrayElements;
   native->GetByteArrayElements = JNIEnv_GetByteArrayElements;
   native->GetCharArrayElements = JNIEnv_GetCharArrayElements;
   native->GetShortArrayElements = JNIEnv_GetShortArrayElements;
   native->GetIntArrayElements = JNIEnv_GetIntArrayElements;
   native->GetLongArrayElements = JNIEnv_GetLongArrayElements;
   native->GetFloatArrayElements = JNIEnv_GetFloatArrayElements;
   native->GetDoubleArrayElements = JNIEnv_GetDoubleArrayElements;
   native->ReleaseBooleanArrayElements = JNIEnv_ReleaseBooleanArrayElements;
   native->ReleaseByteArrayElements = JNIEnv_ReleaseByteArrayElements;
   native->ReleaseCharArrayElements = JNIEnv_ReleaseCharArrayElements;
   native->ReleaseShortArrayElements = JNIEnv_ReleaseShortArrayElements;
   native->ReleaseIntArrayElements = JNIEnv_ReleaseIntArrayElements;
   native->ReleaseLongArrayElements = JNIEnv_ReleaseLongArrayElements;
   native->ReleaseFloatArrayElements = JNIEnv_ReleaseFloatArrayElements;
   native->ReleaseDoubleArrayElements = JNIEnv_ReleaseDoubleArrayElements;
   native->GetBooleanArrayRegion = JNIEnv_GetBooleanArrayRegion;
   native->GetByteArrayRegion = JNIEnv_GetByteArrayRegion;
   native->GetCharArrayRegion = JNIEnv_GetCharArrayRegion;
   native->GetShortArrayRegion = JNIEnv_GetShortArrayRegion;
   native->GetIntArrayRegion = JNIEnv_GetIntArrayRegion;
   native->GetLongArrayRegion = JNIEnv_GetLongArrayRegion;
   native->GetFloatArrayRegion = JNIEnv_GetFloatArrayRegion;
   native->GetDoubleArrayRegion = JNIEnv_GetDoubleArrayRegion;
   native->SetBooleanArrayRegion = JNIEnv_SetBooleanArrayRegion;
   native->SetByteArrayRegion = JNIEnv_SetByteArrayRegion;
   native->SetCharArrayRegion = JNIEnv_SetCharArrayRegion;
   native->SetShortArrayRegion = JNIEnv_SetShortArrayRegion;
   native->SetIntArrayRegion = JNIEnv_SetIntArrayRegion;
   native->SetLongArrayRegion = JNIEnv_SetLongArrayRegion;
   native->SetFloatArrayRegion = JNIEnv_SetFloatArrayRegion;
   native->SetDoubleArrayRegion = JNIEnv_SetDoubleArrayRegion;
   native->RegisterNatives = JNIEnv_RegisterNatives;
   native->UnregisterNatives = JNIEnv_UnregisterNatives;
   native->MonitorEnter = JNIEnv_MonitorEnter;
   native->MonitorExit = JNIEnv_MonitorExit;
   native->GetJavaVM = JNIEnv_GetJavaVM;
   native->GetStringRegion = JNIEnv_GetStringRegion;
   native->GetStringUTFRegion = JNIEnv_GetStringUTFRegion;
   native->GetPrimitiveArrayCritical = JNIEnv_GetPrimitiveArrayCritical;
   native->ReleasePrimitiveArrayCritical = JNIEnv_ReleasePrimitiveArrayCritical;
   native->GetStringCritical = JNIEnv_GetStringCritical;
   native->ReleaseStringCritical = JNIEnv_ReleaseStringCritical;
   native->NewWeakGlobalRef = JNIEnv_NewWeakGlobalRef;
   native->DeleteWeakGlobalRef = JNIEnv_DeleteWeakGlobalRef;
   native->ExceptionCheck = JNIEnv_ExceptionCheck;
   native->NewDirectByteBuffer = JNIEnv_NewDirectByteBuffer;
   native->GetDirectBufferAddress = JNIEnv_GetDirectBufferAddress;
   native->GetDirectBufferCapacity = JNIEnv_GetDirectBufferCapacity;
   *env = native;
}

static jint
JavaVM_DestroyJavaVM(JavaVM *vm)
{
   return 0;
}

static jint
JavaVM_AttachCurrentThread(JavaVM *vm, JNIEnv **env, void *args)
{
   *env = &javavm_get_jvm(vm)->env;
   return 0;
}

static jint
JavaVM_DetachCurrentThread(JavaVM *vm)
{
   return 0;
}

static jint
JavaVM_GetEnv(JavaVM *vm, void **env, jint version)
{
   *env = &javavm_get_jvm(vm)->env;
   return 0;
}

static jint
JavaVM_AttachCurrentThreadAsDaemon(JavaVM *vm, JNIEnv **env, void *args)
{
   *env = &javavm_get_jvm(vm)->env;
   return 0;
}

static void
vm_init(JavaVM *vm, struct JNIInvokeInterface *invoke)
{
   assert(vm && invoke);
   invoke->DestroyJavaVM = JavaVM_DestroyJavaVM;
   invoke->AttachCurrentThread = JavaVM_AttachCurrentThread;
   invoke->DetachCurrentThread = JavaVM_DetachCurrentThread;
   invoke->GetEnv = JavaVM_GetEnv;
   invoke->AttachCurrentThreadAsDaemon = JavaVM_AttachCurrentThreadAsDaemon;
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

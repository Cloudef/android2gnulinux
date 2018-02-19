#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "jvm/jni.h"

jclass
java_lang_Object_getClass(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return (*env)->GetObjectClass(env, object);
}

jobject
java_lang_Class_getClassLoader(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/lang/ClassLoader"))));
}

jobject
java_lang_ClassLoader_findLibrary(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   printf("%s\n", (*env)->GetStringUTFChars(env, str, NULL));
   va_end(args);
   return NULL;
}

jobject
java_util_Map_entrySet(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/util/Set"))));
}

jobject
java_util_Set_iterator(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/util/Iterator"))));
}

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <err.h>
#include <dlfcn.h>
#include "jvm/jni.h"

void
java_lang_System_load(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   const char *lib = (*env)->GetStringUTFChars(env, va_arg(args, jstring), NULL);
   va_end(args);
   printf("%s\n", lib);
   // FIXME: should handle loading properly
   // bionic_dlopen(lib, RTLD_NOW | RTLD_GLOBAL);
}

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
   // XXX: according to docs should return absolute path, but relative is more convenient for us.
   //      fix if breaks anything because of this.
   char lib[255];
   snprintf(lib, sizeof(lib), "lib%s.so", (*env)->GetStringUTFChars(env, va_arg(args, jstring), NULL));
   va_end(args);
   printf("%s\n", lib);
   return (*env)->NewStringUTF(env, lib);
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

jboolean
java_util_Iterator_hasNext(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return false;
}

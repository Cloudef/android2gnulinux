#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <err.h>
#include <dlfcn.h>
#include "jvm/jni.h"
#include "linker/dlfcn.h"
#include "wrapper/verbose.h"

void
java_lang_System_load(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   const char *lib = (*env)->GetStringUTFChars(env, va_arg(args, jstring), NULL);
   va_end(args);
   verbose("%s", lib);

   void *handle = bionic_dlopen(lib, RTLD_NOW | RTLD_GLOBAL);
   assert(handle);

   union {
      void *ptr;
      void* (*fun)(void*, void*);
   } JNI_OnLoad;

   if ((JNI_OnLoad.ptr = bionic_dlsym(handle, "JNI_OnLoad"))) {
      JavaVM *vm;
      (*env)->GetJavaVM(env, &vm);
      JNI_OnLoad.fun(vm, NULL);
   }
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

jstring
java_io_File_getPath(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   // FIXME: see comment on `android_content_Context_getExternalFilesDir`
   return (*env)->NewStringUTF(env, "tmp");
}

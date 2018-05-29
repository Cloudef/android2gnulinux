#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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
   verbose("%s", lib);

   void *handle;
   if (!(handle = bionic_dlopen(lib, RTLD_NOW | RTLD_GLOBAL)))
      return;

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
java_lang_Object_getClass(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->GetObjectClass(env, object);
}

jobject
java_lang_Class_getClassLoader(JNIEnv *env, jobject object)
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
   return (*env)->NewStringUTF(env, lib);
}

jstring
java_io_File_getPath(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   // FIXME: see comment on `android_content_Context_getExternalFilesDir`
#if WOLF
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/local/data/com.swiftappskom.thewolfrpg/files");
#else
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/local/data/com.miHoYo.bh3oversea/files");
#endif
}

jstring
java_io_File_getParent(JNIEnv *env, jobject object, va_list args)
{
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/local/data/com.miHoYo.bh3oversea");
}

jboolean
java_lang_String_equals(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   const char *utf1 = (*env)->GetStringUTFChars(env, object, NULL);
   const char *utf2 = (*env)->GetStringUTFChars(env, str, NULL);
   const jboolean equal = (utf1 == utf2 || (utf1 && utf2 && !strcmp(utf1, utf2)));
   (*env)->ReleaseStringUTFChars(env, object, utf1);
   (*env)->ReleaseStringUTFChars(env, str, utf2);
   return equal;
}

jclass
java_lang_Class_forName(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   const char *utf = (*env)->GetStringUTFChars(env, str, NULL);
   return (*env)->FindClass(env, utf);
}

jstring
java_util_Locale_getLanguage(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "en");
}

jstring
java_util_Locale_getCountry(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "US");
}

#include "jvm/jvm.h"
#include <stdint.h>

jstring
java_lang_Class_getName(JNIEnv *env, jobject object)
{
   assert(env && object);
   const struct jvm *jvm = jnienv_get_jvm(env);
   return (*env)->NewStringUTF(env, jvm->objects[(uintptr_t)object - 1].klass.name.data);
}

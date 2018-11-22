#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <libgen.h>
#include <string.h>
#include <assert.h>
#include <err.h>
#include <dlfcn.h>
#include <err.h>
#include "jvm/jni.h"

jstring
java_lang_System_getProperty(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   const char *key = (*env)->GetStringUTFChars(env, va_arg(args, jstring), NULL);

   if (!strcmp(key, "java.vm.version"))
       return (*env)->NewStringUTF(env, "1.6");

   union {
      void *ptr;
      int (*fun)(const char*, char*);
   } __system_property_get;

   if (!(__system_property_get.ptr = dlsym(RTLD_DEFAULT, "__system_property_get")))
      return NULL;

   char value[92]; // PROP_VALUE_MAX 92
   __system_property_get.fun(key, value);
   return (*env)->NewStringUTF(env, value);
}

void
java_lang_System_load(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   const char *lib = (*env)->GetStringUTFChars(env, va_arg(args, jstring), NULL);

   struct {
      union {
         void *ptr;
         void* (*fun)(const char*, int);
      } open;

      union {
         void *ptr;
         void* (*fun)(void*, const char*);
      } sym;
   } dl;

   if (!(dl.open.ptr = dlsym(RTLD_DEFAULT, "bionic_dlopen")) || !(dl.sym.ptr = dlsym(RTLD_DEFAULT, "bionic_dlsym"))) {
      dl.open.fun = dlopen;
      dl.sym.fun = dlsym;
   }

   void *handle;
   if (!(handle = dl.open.fun(lib, RTLD_NOW | RTLD_GLOBAL))) {
      warnx("java/lang/System/load: failed to dlopen `%s`", lib);
      return;
   }

   union {
      void *ptr;
      void* (*fun)(void*, void*);
   } JNI_OnLoad;

   if ((JNI_OnLoad.ptr = dl.sym.fun(handle, "JNI_OnLoad"))) {
      JavaVM *vm;
      (*env)->GetJavaVM(env, &vm);
      JNI_OnLoad.fun(vm, NULL);
   }
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

jobject
java_lang_Class_getClassLoader(JNIEnv *env, jobject object)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/lang/ClassLoader"))));
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
java_lang_Class_getName(JNIEnv *env, jobject object)
{
   assert(env && object);

   {
      struct {
         union {
            void *ptr;
            struct jvm* (*fun)(JNIEnv*);
         } jnienv_get_jvm;

         union {
            void *ptr;
            const char* (*fun)(const struct jvm*, jobject);
         } jvm_get_class_name;
      } jvm;

      if ((jvm.jnienv_get_jvm.ptr = dlsym(RTLD_DEFAULT, "jnienv_get_jvm")) && (jvm.jvm_get_class_name.ptr = dlsym(RTLD_DEFAULT, "jvm_get_class_name"))) {
         struct jvm *jvm_ = jvm.jnienv_get_jvm.fun(env);
         return (*env)->NewStringUTF(env, jvm.jvm_get_class_name.fun(jvm_, object));
      }
   }

   warnx("%s: returning NULL, as running in unknown JVM and don't know how to get class name", __func__);
   return NULL;
}

jclass
java_lang_Object_getClass(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->GetObjectClass(env, object);
}

jstring
java_io_File_getPath(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   // FIXME: see comment on `android_content_Context_getExternalFilesDir`
   return (*env)->NewStringUTF(env, getenv("ANDROID_EXTERNAL_FILES_DIR"));
}

jstring
java_io_File_getParent(JNIEnv *env, jobject object, va_list args)
{
   // FIXME: see comment on `android_content_Context_getExternalFilesDir`
   char path[4096];
   snprintf(path, sizeof(path), "%s", getenv("ANDROID_EXTERNAL_FILES_DIR"));
   return (*env)->NewStringUTF(env, dirname(path));
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

jbyteArray
java_lang_String_getBytes(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   const char *utf = (*env)->GetStringUTFChars(env, object, NULL);
   const size_t len = (utf ? strlen(utf) : 0);
   jbyteArray bytes = (*env)->NewByteArray(env, len);
   (*env)->SetByteArrayRegion(env, bytes, 0, len, (jbyte*)utf);
   return bytes;
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

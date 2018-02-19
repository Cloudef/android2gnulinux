#include <assert.h>
#include "jvm/jni.h"

jstring
android_content_Context_getPackageName(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "com.gnu.linux");
}

jstring
android_content_context_getPackageName(JNIEnv *env, jobject object, va_list args)
{
   // Probably bug in unity, call to lowercase context class, but works due to case-insensitive filesystem.
   // <https://stackoverflow.com/questions/10890805/case-sensitivity-of-java-class-names>
   return android_content_Context_getPackageName(env, object, args);
}

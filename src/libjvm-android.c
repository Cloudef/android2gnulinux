#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "jvm/jni.h"

jstring
android_content_Context_getPackageName(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "com.gnu.linux");
}

jobject
android_content_Context_getSharedPreferences(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   printf("%s::%d\n", (*env)->GetStringUTFChars(env, str, NULL), va_arg(args, jint));
   va_end(args);
   return (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/SharedPreferences"));
}

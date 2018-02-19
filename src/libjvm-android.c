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
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/SharedPreferences"))));
}

jobject
android_content_SharedPreferences_edit(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/SharedPreferences/Editor"))));
}

jobject
android_content_SharedPreferences_getAll(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/util/Map"))));
}

jobject
android_content_SharedPreferences_Editor_putInt(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   printf("%s::%d\n", (*env)->GetStringUTFChars(env, str, NULL), va_arg(args, jint));
   va_end(args);
   return object;
}

void
android_content_SharedPreferences_Editor_apply(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
}

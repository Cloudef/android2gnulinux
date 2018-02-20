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

jstring
android_content_Context_getPackageCodePath(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, ".");
}

jobject
android_content_Context_getSharedPreferences(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
   printf("%d\n", va_arg(args, jint));
   va_end(args);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/SharedPreferences"))));
}

jint
android_content_SharedPreferences_getInt(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   printf("%s::%d\n", (*env)->GetStringUTFChars(env, str, NULL), va_arg(args, jint));
   va_end(args);
   return 0;
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

jobject
android_content_Context_getExternalFilesDir(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   // FIXME: add mechanism that allows us to implement these objects and then
   //        use `$XDG_DATA_HOME/android2gnulinux/appid` for the path
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
   va_end(args);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/io/File"))));
}

jobject
android_content_Context_getFilesDir(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/io/File"))));
}

jobject
android_app_ApplicationErrorReport_getErrorReportReceiver(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jobject obj = va_arg(args, jobject); // android.content.Context
   jstring str = va_arg(args, jstring);
   jint flags = va_arg(args, jint);
   (*env)->GetStringUTFChars(env, str, NULL);
   printf("%p, %d\n", obj, flags);
   va_end(args);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/ComponentName"))));
}

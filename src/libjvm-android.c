#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "jvm/jni.h"
#include "wrapper/verbose.h"

jstring
android_os_Build_MANUFACTURER(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "android2gnulinux");
}

jstring
android_os_Build_MODEL(JNIEnv *env, jobject object)
{
   return android_os_Build_MANUFACTURER(env, object);
}

jstring
android_os_Build_PRODUCT(JNIEnv *env, jobject object)
{
   return android_os_Build_MANUFACTURER(env, object);
}

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

jstring
android_content_Context_POWER_SERVICE(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "power");
}

jstring
android_content_Context_WINDOW_SERVICE(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "window");
}

jobject
android_content_Context_getSystemService(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
   return NULL;
}

jint
android_content_Context_checkCallingOrSelfPermission(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
   return -1; // PERMISSION_DENIED
}

jobject
android_content_Context_getPackageManager(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/pm/PackageManager"))));
}

jobject
android_content_pm_PackageManager_getApplicationInfo(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   verbose("%s::%d", (*env)->GetStringUTFChars(env, str, NULL), va_arg(args, jint));
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/pm/ApplicationInfo"))));
}

jobject
android_content_Context_getSharedPreferences(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
   verbose("%d", va_arg(args, jint));
   va_end(args);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/SharedPreferences"))));
}

jint
android_content_SharedPreferences_getInt(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   verbose("%s::%d", (*env)->GetStringUTFChars(env, str, NULL), va_arg(args, jint));
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
   verbose("%s::%d", (*env)->GetStringUTFChars(env, str, NULL), va_arg(args, jint));
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
   verbose("%p, %d", obj, flags);
   va_end(args);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "android/content/ComponentName"))));
}

jstring
android_app_Activity_getPackageName(JNIEnv *env, jobject object, va_list args)
{
   // FIXME: Maybe we should be able to handle java inheritance somehow
   //        so we don't have to create these wrapper functions.
   return android_content_Context_getPackageName(env, object, args);
}

jstring
android_net_Uri_encode(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   return str;
}

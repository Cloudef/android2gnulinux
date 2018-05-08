#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "jvm/jni.h"

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
android_os_Build_ID(JNIEnv *env, jobject object)
{
   return android_os_Build_MANUFACTURER(env, object);
}

jstring
android_os_Build_VERSION_RELEASE(JNIEnv *env, jobject object)
{
   assert(env && object);
   // XXX: android2gnulinux version, but we may need to fake this if apps rely on this
   return (*env)->NewStringUTF(env, "1.0");
}

jint
android_os_Build_VERSION_SDK_INT(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (jint)~0; // Always the latest and greatest, XXX: maybe tunable, as can change app behaviour
}

jstring
android_os_Build_VERSION_INCREMENTAL(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "0"); // XXX: maybe git sha of this repo
}

jstring
android_content_Context_getPackageName(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "com.miHoYo.bh3oversea");
}

jstring
android_content_Context_getPackageCodePath(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/apks/honkai");
}

jobject
android_content_Context_getExternalFilesDir(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   // FIXME: add mechanism that allows us to implement these objects and then
   //        use `$XDG_DATA_HOME/android2gnulinux/appid` for the path
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
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

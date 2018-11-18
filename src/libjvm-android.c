#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
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
   return INT_MAX; // Always the latest and greatest, XXX: maybe tunable, as can change app behaviour
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
#if WOLF
   return (*env)->NewStringUTF(env, "com.swiftappskom.thewolfrpg");
#elif STARLIGHT
   return (*env)->NewStringUTF(env, "jp.co.bandainamcoent.BNEI0242");
#elif SHADOWVERSE
   return (*env)->NewStringUTF(env, "com.cygames.Shadowverse");
#elif HEARTHSTONE
   return (*env)->NewStringUTF(env, "com.blizzard.wtcg.hearthstone");
#else
   return (*env)->NewStringUTF(env, "com.miHoYo.bh3oversea");
#endif
}

jstring
android_content_Context_getPackageCodePath(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
#if WOLF
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/apks/wolf");
#elif STARLIGHT
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/apks/starlight");
#elif SHADOWVERSE
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/apks/shadowverse");
#elif HEARTHSTONE
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/apks/hearthstone");
#else
   return (*env)->NewStringUTF(env, "/mnt/media/dev/android2gnulinux/apks/honkai");
#endif
}

jstring
android_content_pm_PackageInfo_versionName(JNIEnv *env, jobject object)
{
   assert(env && object);
   return (*env)->NewStringUTF(env, "1.1");
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

jobject
android_content_Context_getCacheDir(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/io/File"))));
}

jobject
android_content_Context_getExternalCacheDir(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/io/File"))));
}

jstring
android_net_Uri_decode(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
   return str;
}

jstring
android_net_Uri_encode(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str, NULL);
   return str;
}

jstring
android_content_SharedPreferences_getString(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str1 = va_arg(args, jstring);
   jstring str2 = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str1, NULL);
   (*env)->GetStringUTFChars(env, str2, NULL);
   return str2;
}

jstring
android_os_Bundle_getString(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   jstring str1 = va_arg(args, jstring);
   (*env)->GetStringUTFChars(env, str1, NULL);
   return NULL;
}

jintArray
android_view_InputDevice_getDeviceIds(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return (*env)->NewIntArray(env, 1);
}

jint
android_view_InputEvent_getSource(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return 0x00001002; // SOURCE_TOUCHSCREEN
}

jint
android_view_MotionEvent_getAction(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   static int action;
   return (action = !action);
}

jfloat
android_view_MotionEvent_getX(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return 1024/2;
}

jfloat
android_view_MotionEvent_getY(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return 700;
}

jint
android_view_MotionEvent_getPointerCount(JNIEnv *env, jobject object, va_list args)
{
   assert(env && object);
   return 1;
}

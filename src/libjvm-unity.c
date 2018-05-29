#include <stddef.h>
#include <assert.h>
#include "jvm/jni.h"

jmethodID
com_unity3d_player_ReflectionHelper_getMethodID(JNIEnv *env, jobject object, jvalue *values)
{
   assert(env && object);
   const char *utf1 = (*env)->GetStringUTFChars(env, values[1].l, NULL);
   const char *utf2 = (*env)->GetStringUTFChars(env, values[2].l, NULL);
   return (*env)->GetMethodID(env, values[0].l, utf1, utf2);
}

jfieldID
com_unity3d_player_ReflectionHelper_getFieldID(JNIEnv *env, jobject object, jvalue *values)
{
   assert(env && object);
   const char *utf1 = (*env)->GetStringUTFChars(env, values[1].l, NULL);
   const char *utf2 = (*env)->GetStringUTFChars(env, values[2].l, NULL);
   return (*env)->GetFieldID(env, values[0].l, utf1, utf2);
}

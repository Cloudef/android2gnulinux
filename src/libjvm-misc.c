#include <stdlib.h>
#include <stdio.h>
#include "jvm/jni.h"

jstring
com_blizzard_wtcg_hearthstone_DeviceSettings_GetModelNumber(JNIEnv *env, jobject object)
{
   return (*env)->NewStringUTF(env, "0");
}


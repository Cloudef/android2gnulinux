#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "jvm/jni.h"

jobject
bitter_jnibridge_JNIBridge_newInterfaceProxy(JNIEnv *env, jobject object, va_list args)
{
   static jobject sv;
   return (sv ? sv : (sv = (*env)->AllocObject(env, (*env)->FindClass(env, "java/lang/reflect/Proxy"))));
}

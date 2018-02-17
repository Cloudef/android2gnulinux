#pragma once

#include "jni.h"

struct jvm_native_method {
    char *klass;
    char *method;
    void *function;
};

struct jvm {
    struct jvm_native_method methods[255];

    struct JNINativeInterface native;
    struct JNIInvokeInterface invoke;

    // JNI's api is weird.. pointer to a reference of a struct, OK!
    // Developers have to dereference these pointers to call methods from an ... reference.
    // NOTE: These are pointers, and JNI interface passes pointers to these pointers!
    JNIEnv env; // points to native
    JavaVM vm; // points to invoke
};

void
jvm_init(struct jvm *jvm);

// Need android-ndk, compile with:
// i686-linux-android-gcc --sysroot=$ANDROID_NDK/platforms/android-9/arch-x86 -std=c99 -shared -I../src -DANDROID_X86_LINKER -DVERBOSE_FUNCTIONS libwrap.c ../src/wrapper/wrapper.c ../src/jvm/jvm.c -llog -o libwrap.so
// Rename libwrap.so to the library's name you want to middle-man, and the original library to liborig.so
//
// We use logging thread to avoid putting android specific code to verbose/wrapper.h
// Logging thread will catch stdout && stderr.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <pthread.h>
#include <assert.h>
#include <android/log.h>
#include "jvm/jvm.h"
#include "wrapper/wrapper.h"

static int pfd[2];

static void*
log_thread(void *arg)
{
    char buf[255];
    ssize_t r;
    while ((r = read(pfd[0], buf, sizeof(buf) - 1)) > 0) {
        r = (r > 0 && buf[r - 1] == '\n' ? r - 1 : r);
        buf[r] = 0;
        __android_log_write(ANDROID_LOG_INFO, "native", buf);
    }
    return NULL;
}

static void
stdlog_workaround(void)
{
    setvbuf(stdout, 0, _IOLBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
    pipe(pfd);
    dup2(pfd[1], 1);
    dup2(pfd[1], 2);
    pthread_t thread;
    pthread_create(&thread, 0, log_thread, NULL);
    pthread_detach(thread);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved)
{
    stdlog_workaround();

    __android_log_print(ANDROID_LOG_INFO, "native", "Loading liborig.so");
    void *handle = dlopen("liborig.so", RTLD_NOW);
    assert(handle);

    jint (*JNI_OnLoad)(JavaVM*, void*) = dlsym(handle, "JNI_OnLoad");
    assert(JNI_OnLoad);

    struct jvm jvm;
    jvm_init(&jvm);

    __android_log_print(ANDROID_LOG_INFO, "native", "Call liborig's JNI_OnLoad with fakevm");
    JNI_OnLoad(&jvm.vm, NULL);

    // Sort of hack, may fail on some libraries maybe.
    // Some libs store the vm ^ in above call, we want it to use the real vm though.
    // We only pass fake vm in above call to know what natives it registered.
    JNI_OnLoad(vm, NULL);

    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK)
       return -1;

    __android_log_print(ANDROID_LOG_INFO, "native", "Wrapping natives methods");

    jclass old_klass = jvm.methods[0].method.klass;
    JNINativeMethod methods[255];
    for (size_t i = 0, c = 0; c < 255 && i < 255; ++i, ++c) {
        if (jvm.methods[i].method.klass != old_klass) {
            assert(old_klass > 0);
            jclass klass = (*env)->FindClass(env, jvm.objects[(intptr_t)old_klass - 1].klass.name.data);
            (*env)->RegisterNatives(env, klass, methods, c);
            old_klass = jvm.methods[i].method.klass;
            c = 0;
        }

        if (!jvm.methods[i].function)
            break;

        methods[c].name = jvm.methods[i].method.name.data;
        methods[c].signature = jvm.methods[i].method.signature.data;
        methods[c].fnPtr = wrapper_create(methods[c].name, jvm.methods[i].function);
    }

    return JNI_VERSION_1_6;
}

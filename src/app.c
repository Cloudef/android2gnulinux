#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <dlfcn.h>
#include <err.h>
#include <assert.h>
#include "jvm/jvm.h"

extern void *apkenv_android_dlopen(const char*, int);
extern void *apkenv_android_dlclose(void*);
extern const char *apkenv_android_dlerror(void);
extern void *apkenv_android_dlsym(void*, const char*);
extern void apkenv_parse_library_path(const char *path, char *delim);

int
main(int argc, const char *argv[])
{
   if (argc < 2)
      errx(EXIT_FAILURE, "usage: so-file");

   printf("loading runtime\n");
   if (!dlopen("libpthread.so", RTLD_NOW | RTLD_GLOBAL) ||
       !dlopen("libjvm-android.so", RTLD_NOW | RTLD_GLOBAL))
      errx(EXIT_FAILURE, "%s", dlerror());

   printf("loading module: %s\n", argv[1]);

   {
      char path[4096];
      snprintf(path, sizeof(path), "%s", argv[1]);
      apkenv_parse_library_path(dirname(path), ";");
   }

   {
      void *handle;
      if (!(handle = apkenv_android_dlopen(argv[1], RTLD_NOW | RTLD_LOCAL)))
         errx(EXIT_FAILURE, "dlopen failed: %s", apkenv_android_dlerror());

      printf("trying JNI_OnLoad from: %s\n", argv[1]);

      struct jvm jvm;
      jvm_init(&jvm);
      const jobject context = jvm.native.AllocObject(&jvm.env, jvm.native.FindClass(&jvm.env, "android/content/context"));

      void* (*JNI_OnLoad)(void*, void*) = apkenv_android_dlsym(handle, "JNI_OnLoad");
      assert(JNI_OnLoad);

      JNI_OnLoad(&jvm.vm, NULL);

      static const char *unity_player_class = "com/unity3d/player/UnityPlayer";
      void (*native_init)(JNIEnv*, jobject, jobject) = jvm_get_native_method(&jvm, unity_player_class, "initJni");
      void (*native_file)(JNIEnv*, jobject, jstring) = jvm_get_native_method(&jvm, unity_player_class, "nativeFile");
      jboolean (*native_pause)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativePause");
      void (*native_done)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeDone");
      void (*native_recreate_gfx_state)(JNIEnv*, jobject, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeRecreateGfxState");
      void (*native_resume)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeResume");
      jboolean (*native_render)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeRender");
      void (*native_focus_changed)(JNIEnv*, jobject, jboolean) = jvm_get_native_method(&jvm, unity_player_class, "nativeFocusChanged");
      void (*native_set_input_string)(JNIEnv*, jobject, jstring) = jvm_get_native_method(&jvm, unity_player_class, "nativeSetInputString");
      void (*native_soft_input_closed)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeSoftInputClosed");
      void (*native_soft_input_canceled)(JNIEnv*, jobject, jboolean) = jvm_get_native_method(&jvm, unity_player_class, "nativeSoftInputCanceled");
      native_init(&jvm.env, context, context);
      native_file(&jvm.env, context, jvm.native.NewStringUTF(&jvm.env, "./file.apk"));
      native_done(&jvm.env, context);
      native_recreate_gfx_state(&jvm.env, context, context);
      native_render(&jvm.env, (jobject)1);

      printf("unloading module: %s\n", argv[1]);
      apkenv_android_dlclose(handle);
      jvm_release(&jvm);
   }

   printf("exiting\n");
   return EXIT_SUCCESS;
}

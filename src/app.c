#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <dlfcn.h>
#include <err.h>
#include <assert.h>
#include <linux/limits.h>
#include "linker/dlfcn.h"
#include "jvm/jvm.h"

int
main(int argc, const char *argv[])
{
   if (argc < 2)
      errx(EXIT_FAILURE, "usage: so-file");

   printf("loading module: %s\n", argv[1]);

   {
      // FIXME: when bionic linker is rewritten it will just use system search path
      char abs[PATH_MAX];
      realpath(argv[1], abs);
      dl_parse_library_path(dirname(abs), ";");
   }

   {
      void *handle;
      if (!(handle = bionic_dlopen(argv[1], RTLD_NOW | RTLD_LOCAL)))
         errx(EXIT_FAILURE, "dlopen failed: %s", bionic_dlerror());

      printf("trying JNI_OnLoad from: %s\n", argv[1]);

      struct jvm jvm;
      jvm_init(&jvm);
      const jobject context = jvm.native.AllocObject(&jvm.env, jvm.native.FindClass(&jvm.env, "android/app/Activity"));

      jint (*JNI_OnLoad)(void*, void*) = bionic_dlsym(handle, "JNI_OnLoad");
      assert(JNI_OnLoad);

      JNI_OnLoad(&jvm.vm, NULL);

      static const char *unity_player_class = "com.unity3d.player.UnityPlayer";
      void (*native_init_jni)(JNIEnv*, jobject, jobject) = jvm_get_native_method(&jvm, unity_player_class, "initJni");
      void (*native_done)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeDone");
      void (*native_file)(JNIEnv*, jobject, jstring) = jvm_get_native_method(&jvm, unity_player_class, "nativeFile");
      // jboolean (*native_pause)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativePause");
      void (*native_recreate_gfx_state)(JNIEnv*, jobject, jint, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeRecreateGfxState");
      jboolean (*native_render)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeRender");
      void (*native_resume)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeResume");
      void (*native_focus_changed)(JNIEnv*, jobject, jboolean) = jvm_get_native_method(&jvm, unity_player_class, "nativeFocusChanged");
      // void (*native_set_input_string)(JNIEnv*, jobject, jstring) = jvm_get_native_method(&jvm, unity_player_class, "nativeSetInputString");
      // void (*native_soft_input_closed)(JNIEnv*, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeSoftInputClosed");
      // void (*native_set_input_canceled)(JNIEnv*, jobject, jboolean) = jvm_get_native_method(&jvm, unity_player_class, "nativeSetInputCanceled");
      void (*native_init_www)(JNIEnv*, jobject, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeInitWWW");
      void (*native_init_web_request)(JNIEnv*, jobject, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeInitWebRequest");
      // void (*native_add_vsync_time)(JNIEnv*, jobject, jlong) = jvm_get_native_method(&jvm, unity_player_class, "nativeAddVSyncTime");
      // void (*native_forward_events_to_dalvik)(JNIEnv*, jobject, jboolean) = jvm_get_native_method(&jvm, unity_player_class, "nativeForwardEventsToDalvik");
      void (*native_inject_event)(JNIEnv*, jobject, jobject) = jvm_get_native_method(&jvm, unity_player_class, "nativeInjectEvent");
      native_init_jni(&jvm.env, context, context);
#if WOLF
      native_file(&jvm.env, context, jvm.env->NewStringUTF(&jvm.env, "/mnt/media/dev/android2gnulinux/apks/wolf.apk"));
#else
      native_file(&jvm.env, context, jvm.env->NewStringUTF(&jvm.env, "/mnt/media/dev/android2gnulinux/apks/honkai.apk"));
      native_file(&jvm.env, context, jvm.env->NewStringUTF(&jvm.env, "/mnt/media/dev/android2gnulinux/local/obb/com.miHoYo.bh3oversea/main.100.com.miHoYo.bh3oversea.obb"));
#endif
      // native_forward_events_to_dalvik(&jvm.env, context, true);
      native_init_www(&jvm.env, context, jvm.native.FindClass(&jvm.env, "com/unity3d/player/WWW"));
      native_init_web_request(&jvm.env, context, jvm.native.FindClass(&jvm.env, "com/unity3d/player/UnityWebRequest"));
      native_recreate_gfx_state(&jvm.env, context, 0, context);
      native_focus_changed(&jvm.env, context, true);
      native_resume(&jvm.env, context);
      native_done(&jvm.env, context);
      // native_add_vsync_time(&jvm.env, context, 0);

      while (native_render(&jvm.env, context)) {
         static int i = 0;
         if (++i >= 10) {
            native_inject_event(&jvm.env, context, jvm.native.AllocObject(&jvm.env, jvm.native.FindClass(&jvm.env, "android/view/MotionEvent")));
            i = 0;
         }
      }

      printf("unloading module: %s\n", argv[1]);
      bionic_dlclose(handle);
      jvm_release(&jvm);
   }

   printf("exiting\n");
   return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <dlfcn.h>
#include <err.h>
#include <limits.h>
#include "linker/dlfcn.h"
#include "jvm/jvm.h"

static int
run_jni_game(struct jvm *jvm)
{
   // Works only with unity libs for now
   struct {
      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jobject);
      } native_init_jni;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject);
      } native_done;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jstring);
      } native_file;

      union {
         void *ptr;
         jboolean (*fun)(JNIEnv*, jobject);
      } native_pause;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jint, jobject);
      } native_recreate_gfx_state;

      union {
         void *ptr;
         jboolean (*fun)(JNIEnv*, jobject);
      } native_render;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject);
      } native_resume;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jboolean);
      } native_focus_changed;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jstring);
      } native_set_input_string;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject);
      } native_soft_input_closed;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jboolean);
      } native_set_input_canceled;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jobject);
      } native_init_www;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jobject);
      } native_init_web_request;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jlong);
      } native_add_vsync_time;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jboolean);
      } native_forward_events_to_dalvik;

      union {
         void *ptr;
         void (*fun)(JNIEnv*, jobject, jobject);
      } native_inject_event;
   } unity;

   static const char *unity_player_class = "com.unity3d.player.UnityPlayer";
   unity.native_init_jni.ptr = jvm_get_native_method(jvm, unity_player_class, "initJni");
   unity.native_done.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeDone");
   unity.native_file.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeFile");
   unity.native_pause.ptr = jvm_get_native_method(jvm, unity_player_class, "nativePause");
   unity.native_recreate_gfx_state.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeRecreateGfxState");
   unity.native_render.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeRender");
   unity.native_resume.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeResume");
   unity.native_focus_changed.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeFocusChanged");
   unity.native_set_input_string.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeSetInputString");
   unity.native_soft_input_closed.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeSoftInputClosed");
   unity.native_set_input_canceled.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeSetInputCanceled");
   unity.native_init_www.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeInitWWW");
   unity.native_init_web_request.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeInitWebRequest");
   unity.native_add_vsync_time.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeAddVSyncTime");
   unity.native_forward_events_to_dalvik.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeForwardEventsToDalvik");
   unity.native_inject_event.ptr = jvm_get_native_method(jvm, unity_player_class, "nativeInjectEvent");

   if (!unity.native_init_jni.ptr || !unity.native_file.ptr)
      errx(EXIT_FAILURE, "not a unity jni lib");

   const jobject context = jvm->native.AllocObject(&jvm->env, jvm->native.FindClass(&jvm->env, "android/app/Activity"));
   unity.native_init_jni.fun(&jvm->env, context, context);

#if WOLF
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/apks/wolf.apk"));
#elif STARLIGHT
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/apks/starlight.apk"));
#elif STAROCEAN
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/apks/starocean.apk"));
#elif SHADOWVERSE
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/apks/shadowverse.apk"));
#elif HEARTHSTONE
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/apks/hearthstone.apk"));
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/local/obb/com.blizzard.wtcg.hearthstone/patch.1561502.com.blizzard.wtcg.hearthstone.obb"));
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/local/obb/com.blizzard.wtcg.hearthstone/main.1561502.com.blizzard.wtcg.hearthstone.obb"));
#else
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/apks/honkai.apk"));
   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, "/mnt/media/dev/android2gnulinux/local/obb/com.miHoYo.bh3oversea/main.100.com.miHoYo.bh3oversea.obb"));
#endif

   // unity.native_forward_events_to_dalvik.fun(&jvm->env, context, true);
   unity.native_init_www.fun(&jvm->env, context, jvm->env->FindClass(&jvm->env, "com/unity3d/player/WWW"));
   unity.native_init_web_request.fun(&jvm->env, context, jvm->env->FindClass(&jvm->env, "com/unity3d/player/UnityWebRequest"));
   unity.native_recreate_gfx_state.fun(&jvm->env, context, 0, context);
   unity.native_focus_changed.fun(&jvm->env, context, true);
   unity.native_resume.fun(&jvm->env, context);
   unity.native_done.fun(&jvm->env, context);
   // unity.native_add_vsync_time.fun(&jvm->env, context, 0);

   while (unity.native_render.fun(&jvm->env, context)) {
      static int i = 0;
      if (++i >= 10) {
         unity.native_inject_event.fun(&jvm->env, context, jvm->native.AllocObject(&jvm->env, jvm->native.FindClass(&jvm->env, "android/view/MotionEvent")));
         i = 0;
      }
   }

   return EXIT_SUCCESS;
}

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

   void *handle;
   if (!(handle = bionic_dlopen(argv[1], RTLD_LOCAL | RTLD_NOW)))
      errx(EXIT_FAILURE, "dlopen failed: %s", bionic_dlerror());

   printf("trying JNI_OnLoad from: %s\n", argv[1]);

   struct {
      union {
         void *ptr;
         jint (*fun)(void*, void*);
      } JNI_OnLoad;

      union {
         void *ptr;
         int (*fun)(int, const char*[]);
      } main;
   } entry;

   int ret = EXIT_FAILURE;
   if ((entry.JNI_OnLoad.ptr = bionic_dlsym(handle, "JNI_OnLoad"))) {
      struct jvm jvm;
      jvm_init(&jvm);
      entry.JNI_OnLoad.fun(&jvm.vm, NULL);
      ret = run_jni_game(&jvm);
      jvm_release(&jvm);
   } else if ((entry.main.ptr = bionic_dlsym(handle, "main"))) {
      ret = entry.main.fun(argc - 1, &argv[1]);
   } else {
      warnx("no entrypoint found in %s", argv[1]);
   }

   printf("unloading module: %s\n", argv[1]);
   bionic_dlclose(handle);
   printf("exiting\n");
   return ret;
}

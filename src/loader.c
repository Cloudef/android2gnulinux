#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <dlfcn.h>
#include <elf.h>
#include <err.h>
#include <limits.h>
#include "linker/dlfcn.h"
#include "linker/linker.h"
#include "jvm/jvm.h"
#include <link.h>

static int
run_jni_game(struct jvm *jvm)
{
   // Works only with unity libs for now
   // XXX: What this basically is that, we port the Java bits to C
   // XXX: This will become unneccessary as we make dalvik interpreter

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

   unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, getenv("ANDROID_PACKAGE_CODE_PATH")));

   {
      DIR *dir;
      const char *obb_dir = getenv("ANDROID_EXTERNAL_OBB_DIR");
      if (obb_dir && (dir = opendir(obb_dir))) {
         for (struct dirent *d; (d = readdir(dir));) {
            if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
               continue;

            char path[4096];
            snprintf(path, sizeof(path), "%s/%s", obb_dir, d->d_name);
            unity.native_file.fun(&jvm->env, context, jvm->env->NewStringUTF(&jvm->env, path));
         }
      }
   }

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

static void
raw_start(void *entry, int argc, const char *argv[])
{
   // XXX: make this part of the linker when it's rewritten
#if ANDROID_X86_LINKER
   __asm__("mov 2*4(%ebp),%eax"); /* entry */
   __asm__("mov 3*4(%ebp),%ecx"); /* argc */
   __asm__("mov 4*4(%ebp),%edx"); /* argv */
   __asm__("mov %edx,%esp"); /* trim stack. */
   __asm__("push %edx"); /* push argv */
   __asm__("push %ecx"); /* push argc */
   __asm__("sub %edx,%edx"); /* no rtld_fini function */
   __asm__("jmp *%eax"); /* goto entry */
#else
   warnx("raw_start not implemented for this asm platform, can't execute binaries.");
#endif
}

int
main(int argc, const char *argv[])
{
   if (argc < 2)
      errx(EXIT_FAILURE, "usage: <elf file or jni library>");

   printf("loading module: %s\n", argv[1]);

   {
      // FIXME: when bionic linker is rewritten it will just use system search path
      char abs[PATH_MAX], paths[4096];
      realpath(argv[1], abs);
      snprintf(paths, sizeof(paths), "%s:%s", dirname(abs), "runtime-ndk");
      dl_parse_library_path(paths, ":");
   }

   void *handle;
   if (!(handle = bionic_dlopen(argv[1], RTLD_LOCAL | RTLD_NOW)))
      errx(EXIT_FAILURE, "dlopen failed: %s", bionic_dlerror());

   struct {
      union {
         void *ptr;
         jint (*fun)(void*, void*);
      } JNI_OnLoad;

      union {
         void *ptr;
      } start;
   } entry = {0};

   {
      union {
         char bytes[sizeof(Elf32_Ehdr)];
         Elf32_Ehdr hdr;
      } elf;

      FILE *f;
      if (!(f = fopen(argv[1], "rb")))
         err(EXIT_FAILURE, "fopen(%s)", argv[1]);

      fread(elf.bytes, 1, sizeof(elf.bytes), f);
      fclose(f);

      struct soinfo *si = handle;
      if (elf.hdr.e_entry)
         entry.start.ptr = (void*)(intptr_t)(si->base + elf.hdr.e_entry);
   }

   int ret = EXIT_FAILURE;
   if (entry.start.ptr) {
      printf("jumping to %p\n", entry.start.ptr);
      raw_start(entry.start.ptr, argc - 1, &argv[1]);
   } else if ((entry.JNI_OnLoad.ptr = bionic_dlsym(handle, "JNI_OnLoad"))) {
      struct jvm jvm;
      jvm_init(&jvm);
      entry.JNI_OnLoad.fun(&jvm.vm, NULL);
      ret = run_jni_game(&jvm);
      jvm_release(&jvm);
   } else {
      warnx("no entrypoint found in %s", argv[1]);
   }

   printf("unloading module: %s\n", argv[1]);
   bionic_dlclose(handle);
   printf("exiting\n");
   return ret;
}

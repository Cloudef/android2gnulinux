#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <dlfcn.h>
#include <err.h>
#include <assert.h>

extern void *apkenv_android_dlopen(const char*, int);
extern void *apkenv_android_dlclose(void*);
extern void *apkenv_android_dlsym(void*, const char*);
extern void apkenv_parse_library_path(const char *path, char *delim);

int
main(int argc, const char *argv[])
{
   if (argc < 2)
      errx(EXIT_FAILURE, "usage: so-file");

   dlopen("libpthread.so", RTLD_NOW | RTLD_GLOBAL);

   printf("loading: %s\n", argv[1]);

   char path[4096];
   snprintf(path, sizeof(path), "%s", argv[1]);
   apkenv_parse_library_path(dirname(path), ";");

   void *handle;
   if (!(handle = apkenv_android_dlopen(argv[1], RTLD_NOW | RTLD_LOCAL)))
      errx(EXIT_FAILURE, "dlopen failed: %s", dlerror());

   printf("loaded: %s\n", argv[1]);

   void* (*JNI_OnLoad)(void*, void*) = apkenv_android_dlsym(handle, "JNI_OnLoad");
   assert(JNI_OnLoad);

   char data[1024];
   JNI_OnLoad(data, NULL);

   // apkenv_android_dlclose(handle);
   return EXIT_SUCCESS;
}

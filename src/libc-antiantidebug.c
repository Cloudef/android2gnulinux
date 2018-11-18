#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

int
bionic_open(const char *path, int oflag, ...)
{
   // Hide TracerPid from /proc/self/status for hideous apps that check for debugger.
   // Note, since /proc/self/status doesn't get updated anymore, this may break some stuff.
   // XXX: Turn this ON/OFF with env var maybe?
   if (!strcmp(path, "/proc/self/status")) {
      static FILE *faked = NULL;

      if (!faked) {
         static char status[4096];

         {
            FILE *f = fopen(path, "rb");
            assert(f && "/proc/self/status failed to open :/");
            const size_t ret = fread(status, 1, sizeof(status), f);
            assert(ret <= sizeof(status) && "/proc/self/status doesn't fit in 4096 bytes :/");
            fclose(f);
         }

         for (char *s, *e; (s = strstr(status, "TracerPid:\t"));) {
            for (e = s; (size_t)(e - status) < sizeof(status) && *e && *e != '\n'; ++e);
            memmove(s, e, sizeof(status) - (e - status));
            break;
         }

         faked = fmemopen(status, sizeof(status), "rb");
         assert(faked && "fmemopen failed :/");
      }

      return fileno(faked);
   }

   return open(path, oflag);
}

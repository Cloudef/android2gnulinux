#include <stdio.h>

struct bionic___sFILE {
#if defined(__LP64__)
   char __private[152];
#else
   char __private[84];
#endif
} __attribute__((aligned(sizeof(void*))));

// Bionic standard stream support pre-M Android
// Post-M it's saner and they point to stdin/stdout/stderr symbols instead
const struct bionic___sFILE bionic___sF[3] = {
   {{ 's', 't', 'd', 'i', 'n' }},
   {{ 's', 't', 'd', 'o', 'u', 't' }},
   {{ 's', 't', 'd', 'e', 'r', 'r' }}
};

static inline FILE*
bionic_file_to_glibc_file(FILE *f)
{
   if (f == (void*)&bionic___sF[0])
      return stdin;
   else if (f == (void*)&bionic___sF[1])
      return stdout;
   else if (f == (void*)&bionic___sF[2])
      return stderr;
   return f;
}

int
bionic_fflush(FILE *f)
{
   return fflush(bionic_file_to_glibc_file(f));
}

// Wrapping internal glibc VTABLE functions to handle bionic's pre-M crap
// We define __real_IO_file_xsputn in libc.c so linker will link our library,
// it's not used however for anything.

extern size_t
__real_IO_file_xsputn(FILE *f, const void *buf, size_t n);

size_t
__wrap_IO_file_xsputn(FILE *f, const void *buf, size_t n)
{
   return __real_IO_file_xsputn(bionic_file_to_glibc_file(f), buf, n);
}

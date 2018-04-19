#pragma once

#include <wchar.h>

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
   // Can't compare just memory addresses because GNU libstdc++ copies the FILE struct...
   // Maybe there could be a cleaner solution, this may in practice break if standard streams
   // are opened with different flags in C++.
   if (memcmp(f, "stdin", sizeof("stdin")))
      return stdin;
   else if (memcmp(f, "stdout", sizeof("stdout")))
      return stdout;
   else if (memcmp(f, "stderr", sizeof("stderr")))
      return stderr;
   return f;
}

FILE*
bionic_freopen(const char *filename, const char *modes, FILE *stream)
{
   return freopen(filename, modes, bionic_file_to_glibc_file(stream));
}

int
bionic_fclose(FILE *stream)
{
   return fclose(bionic_file_to_glibc_file(stream));
}

int
bionic_fputc(int c, FILE *stream)
{
   return fputc(c, bionic_file_to_glibc_file(stream));
}

int
bionic_putc(int c, FILE *stream)
{
   return putc(c, bionic_file_to_glibc_file(stream));
}


int
bionic_fputs(const char *c, FILE *stream)
{
   return fputs(c, bionic_file_to_glibc_file(stream));
}

wint_t
bionic_fputwc(wchar_t wc, FILE *stream)
{
   return fputwc(wc, bionic_file_to_glibc_file(stream));
}

wint_t
bionic_putwc(wchar_t wc, FILE *stream)
{
   return putwc(wc, bionic_file_to_glibc_file(stream));
}

size_t
bionic_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
   char buf[256], fname[256] = {0};
   stream = bionic_file_to_glibc_file(stream);
   snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fileno(stream));
   readlink(buf, fname, sizeof(fname));
   verbose("%s (%d)\n%p, %zu, %zu, %p", fname, fileno(stream), ptr, size, nmemb, (void*)stream);
   return fread(ptr, size, nmemb, stream);
}

size_t
bionic_fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
   char buf[256], fname[256] = {0};
   stream = bionic_file_to_glibc_file(stream);
   snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fileno(stream));
   readlink(buf, fname, sizeof(fname));
   verbose("%s (%d)\n%p, %zu, %zu, %p", fname, fileno(stream), ptr, size, nmemb, (void*)stream);
   return fwrite(ptr, size, nmemb, stream);
}

int
bionic_fflush(FILE *stream)
{
   return fflush(bionic_file_to_glibc_file(stream));
}

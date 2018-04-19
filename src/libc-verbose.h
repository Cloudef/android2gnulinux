#pragma once

#include <stdarg.h>
#include <string.h>

/**
 * This file implements functions only for verbose output, which is useful for debugging.
 * These functions can be enabled with -DVERBOSE_FUNCTIONS
 */

int
bionic_chdir(const char *path)
{
   verbose("%s", path);
   return chdir(path);
}

int
bionic_rename(const char *old, const char *fresh)
{
   verbose("%s -> %s", old, fresh);
   return rename(old, fresh);
}

int
bionic_access(const char *path, int amode)
{
   verbose("%s (%d)", path, amode);
   return access(path, amode);
}

FILE*
bionic_fopen(const char *path,  const char *mode)
{
   verbose("%s %s", path, mode);
   return fopen(path, mode);
}

int
bionic_mkdir(const char *path, mode_t mode)
{
   verbose("%s -> %u", path, mode);
   return mkdir(path, mode);
}

DIR*
bionic_opendir(const char *path)
{
   verbose("%s", path);
   return opendir(path);
}

int
bionic_sprintf(char *str, const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   int r = vsprintf(str, fmt, ap);
   va_end(ap);
   verbose("%s == %s", fmt, str);
   return r;
}

int
bionic_snprintf(char *str, size_t size, const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   int r = vsnprintf(str, size, fmt, ap);
   va_end(ap);
   verbose("%s (%zu) == %s", fmt, size, str);
   return r;
}

size_t
bionic_strlen(const char *str)
{
   verbose("%s (%zu)", str, strlen(str));
   return strlen(str);
}

char*
bionic_strchr(const char *str, int c)
{
   verbose("%s %c", str, c);
   return strchr(str, c);
}

char*
bionic_strcpy(char *dest, const char *src)
{
   verbose("%s", src);
   return strcpy(dest, src);
}

char*
bionic_strncpy(char *dest, const char *src, size_t n)
{
   verbose("%s (%zu)", src, n);
   return strncpy(dest, src, n);
}

extern char* strdup(const char*);

char*
bionic_strdup(const char *str)
{
   verbose("%s", str);
   return strdup(str);
}

char*
bionic_strstr(const char *haystack, const char *needle)
{
   verbose("%s, %s", haystack, needle);
   return strstr(haystack, needle);
}

int
bionic_strcmp(const char *s1, const char *s2)
{
   verbose("%s == %s", s1, s2);
   return strcmp(s1, s2);
}

int
bionic_strncmp(const char *s1, const char *s2, size_t n)
{
   verbose("%s == %s (%zu)", s1, s2, n);
   return strncmp(s1, s2, n);
}

ssize_t
bionic_readlink(const char *path, char *buf, size_t bufsize)
{
   verbose("%s", path);
   return readlink(path, buf, bufsize);
}

int
bionic_unlink(const char *path)
{
   verbose("%s", path);
   return unlink(path);
}

char*
bionic_getenv(const char *name)
{
   verbose("%s", name);
   return getenv(name);
}

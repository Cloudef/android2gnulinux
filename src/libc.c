#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>

// Stuff that doesn't exist in glibc

void
__assert2(const char* file, int line, const char* function, const char* failed_expression)
{
   fprintf(stderr, "%s:%d: %s: assertion \"%s\" failed", file, line, function, failed_expression);
   abort();
}

pid_t
gettid(void)
{
   return syscall(SYS_gettid);
}

int
tgkill(int tgid, int tid, int sig)
{
   return syscall(SYS_tgkill, tgid, tid, sig);
}

int
tkill(int tid, int sig)
{
   return syscall(SYS_tkill, tid, sig);
}

#include "libc-sha1.h"

// Stuff needed for runtime compatibility, but not neccessary for linking
// Also stuff that exists in glibc, but needs to be wrapped for runtime compatibility

const char *bionic__ctype_, *bionic__tolower_tab_, *bionic__toupper_tab_;
char bionic___sF[0x54];
int bionic___errno;

int
bionic_stat(const char *restrict path, struct stat *restrict buf)
{
   return stat(path, buf);
}

int
bionic_lstat(const char *restrict path, struct stat *restrict buf)
{
   return lstat(path, buf);
}

int
bionic_fstat(int fd, struct stat *buf)
{
   return fstat(fd, buf);
}

int
bionic___isfinitef(float f)
{
   return isfinite(f);
}

uintptr_t bionic___stack_chk_guard = 4;

__attribute__((noreturn))
void bionic___stack_chk_fail(void)
{
   abort();
}

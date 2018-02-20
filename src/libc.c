#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>

struct bionic_dirent {
   uint64_t d_ino;
   int64_t d_off;
   unsigned short d_reclen;
   unsigned char d_type;
   char d_name[256];
};

struct bionic_sigaction {
};

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
char bionic___sF[0x54] = {0};

__attribute_const__
int*
bionic___errno(void)
{
   return __errno_location();
}

int
bionic_stat(const char *restrict path, struct stat *restrict buf)
{
   printf("stat: %s\n", path);
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
bionic_fstatat(int dirfd, const char *pathname, void *buf, int flags)
{
   return fstatat(dirfd, pathname, buf, flags);
}

static void
glibc_dirent_to_bionic_dirent(const struct dirent *de, struct bionic_dirent *bde)
{
   assert(bde && de);
   *bde = (struct bionic_dirent){
      .d_ino = de->d_ino,
      .d_off = de->d_off,
      .d_reclen = de->d_reclen,
      .d_type = de->d_type,
   };
   _Static_assert(sizeof(bde->d_name) >= sizeof(de->d_name), "bionic_dirent can't hold dirent's d_name");
   memcpy(bde->d_name, de->d_name, sizeof(bde->d_name));
}

struct bionic_dirent*
bionic_readdir(DIR *dirp)
{
   assert(dirp);
   static struct bionic_dirent bde;
   struct dirent *de;
   if (!(de = readdir(dirp)))
      return NULL;
   glibc_dirent_to_bionic_dirent(de, &bde);
   return &bde;
}

int
bionic_readdir_r(DIR *dirp, struct bionic_dirent *entry, struct bionic_dirent **result)
{
   assert(dirp && entry && result);
   struct dirent de, *der = NULL;

   int ret;
   if ((ret = readdir_r(dirp, &de, &der)) != 0 || !der) {
      *result = NULL;
      return ret;
   }

   glibc_dirent_to_bionic_dirent(der, entry);
   *result = entry;
   return 0;
}

int
bionic_sigaction(int sig, const struct bionic_sigaction *restrict act, struct bionic_sigaction *restrict oact)
{
   // FIXME: implement
   return 0;
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

static inline int
bionic_sysconf_to_glibc_sysconf(int name)
{
   switch(name) {
      case 0x0000: return _SC_ARG_MAX;
      case 0x0001: return _SC_BC_BASE_MAX;
      case 0x0002: return _SC_BC_DIM_MAX;
      case 0x0003: return _SC_BC_SCALE_MAX;
      case 0x0004: return _SC_BC_STRING_MAX;
      case 0x0005: return _SC_CHILD_MAX;
      case 0x0006: return _SC_CLK_TCK;
      case 0x0007: return _SC_COLL_WEIGHTS_MAX;
      case 0x0008: return _SC_EXPR_NEST_MAX;
      case 0x0009: return _SC_LINE_MAX;
      case 0x000a: return _SC_NGROUPS_MAX;
      case 0x000b: return _SC_OPEN_MAX;
      case 0x000c: return _SC_PASS_MAX;
      case 0x000d: return _SC_2_C_BIND;
      case 0x000e: return _SC_2_C_DEV;
      case 0x000f: return _SC_2_C_VERSION;
      case 0x0010: return _SC_2_CHAR_TERM;
      case 0x001d: return _SC_XOPEN_CRYPT;
      case 0x001e: return _SC_XOPEN_ENH_I18N;
      case 0x001f: return _SC_XOPEN_SHM;
      case 0x0020: return _SC_XOPEN_VERSION;
      case 0x0021: return _SC_XOPEN_XCU_VERSION;
      case 0x0022: return _SC_XOPEN_REALTIME;
      case 0x0023: return _SC_XOPEN_REALTIME_THREADS;
      case 0x0024: return _SC_XOPEN_LEGACY;
      case 0x0025: return _SC_ATEXIT_MAX;
      case 0x0026: return _SC_IOV_MAX;
      case 0x0027: return _SC_PAGESIZE;
      case 0x0028: return _SC_PAGE_SIZE;
      case 0x0029: return _SC_XOPEN_UNIX;
      case 0x002a: return _SC_XBS5_ILP32_OFF32;
      case 0x002b: return _SC_XBS5_ILP32_OFFBIG;
      case 0x002c: return _SC_XBS5_LP64_OFF64;
      case 0x002d: return _SC_XBS5_LPBIG_OFFBIG;
      case 0x002e: return _SC_AIO_LISTIO_MAX;
      case 0x002f: return _SC_AIO_MAX;
      case 0x0030: return _SC_AIO_PRIO_DELTA_MAX;
      case 0x0031: return _SC_DELAYTIMER_MAX;
      case 0x0032: return _SC_MQ_OPEN_MAX;
      case 0x0033: return _SC_MQ_PRIO_MAX;
      case 0x0034: return _SC_RTSIG_MAX;
      case 0x0035: return _SC_SEM_NSEMS_MAX;
      case 0x0036: return _SC_SEM_VALUE_MAX;
      case 0x0037: return _SC_SIGQUEUE_MAX;
      case 0x0038: return _SC_TIMER_MAX;
      case 0x0039: return _SC_ASYNCHRONOUS_IO;
      case 0x003a: return _SC_FSYNC;
      case 0x003b: return _SC_MAPPED_FILES;
      case 0x003c: return _SC_MEMLOCK;
      case 0x003d: return _SC_MEMLOCK_RANGE;
      case 0x003e: return _SC_MEMORY_PROTECTION;
      case 0x003f: return _SC_MESSAGE_PASSING;
      case 0x0040: return _SC_PRIORITIZED_IO;
      case 0x0041: return _SC_PRIORITY_SCHEDULING;
      case 0x0042: return _SC_REALTIME_SIGNALS;
      case 0x0043: return _SC_SEMAPHORES;
      case 0x0044: return _SC_SHARED_MEMORY_OBJECTS;
      case 0x0045: return _SC_SYNCHRONIZED_IO;
      case 0x0046: return _SC_TIMERS;
      case 0x0047: return _SC_GETGR_R_SIZE_MAX;
      case 0x0048: return _SC_GETPW_R_SIZE_MAX;
      case 0x0049: return _SC_LOGIN_NAME_MAX;
      case 0x004a: return _SC_THREAD_DESTRUCTOR_ITERATIONS;
      case 0x004b: return _SC_THREAD_KEYS_MAX;
      case 0x004c: return _SC_THREAD_STACK_MIN;
      case 0x004d: return _SC_THREAD_THREADS_MAX;
      case 0x004e: return _SC_TTY_NAME_MAX;
      case 0x004f: return _SC_THREADS;
      case 0x0050: return _SC_THREAD_ATTR_STACKADDR;
      case 0x0051: return _SC_THREAD_ATTR_STACKSIZE;
      case 0x0052: return _SC_THREAD_PRIORITY_SCHEDULING;
      case 0x0053: return _SC_THREAD_PRIO_INHERIT;
      case 0x0054: return _SC_THREAD_PRIO_PROTECT;
      case 0x0055: return _SC_THREAD_SAFE_FUNCTIONS;
      case 0x0060: return _SC_NPROCESSORS_CONF;
      case 0x0061: return _SC_NPROCESSORS_ONLN;
      case 0x0062: return _SC_PHYS_PAGES;
      case 0x0063: return _SC_AVPHYS_PAGES;
      case 0x0064: return _SC_MONOTONIC_CLOCK;
      case 0x0065: return _SC_2_PBS;
      case 0x0066: return _SC_2_PBS_ACCOUNTING;
      case 0x0067: return _SC_2_PBS_CHECKPOINT;
      case 0x0068: return _SC_2_PBS_LOCATE;
      case 0x0069: return _SC_2_PBS_MESSAGE;
      case 0x006a: return _SC_2_PBS_TRACK;
      case 0x006b: return _SC_ADVISORY_INFO;
      case 0x006c: return _SC_BARRIERS;
      case 0x006d: return _SC_CLOCK_SELECTION;
      case 0x006e: return _SC_CPUTIME;
      case 0x006f: return _SC_HOST_NAME_MAX;
      case 0x0070: return _SC_IPV6;
      case 0x0071: return _SC_RAW_SOCKETS;
      case 0x0072: return _SC_READER_WRITER_LOCKS;
      case 0x0073: return _SC_REGEXP;
      case 0x0074: return _SC_SHELL;
      case 0x0075: return _SC_SPAWN;
      case 0x0076: return _SC_SPIN_LOCKS;
      case 0x0077: return _SC_SPORADIC_SERVER;
      case 0x0078: return _SC_SS_REPL_MAX;
      case 0x0079: return _SC_SYMLOOP_MAX;
      case 0x007a: return _SC_THREAD_CPUTIME;
      case 0x007b: return _SC_THREAD_PROCESS_SHARED;
      case 0x007c: return _SC_THREAD_ROBUST_PRIO_INHERIT;
      case 0x007d: return _SC_THREAD_ROBUST_PRIO_PROTECT;
      case 0x007e: return _SC_THREAD_SPORADIC_SERVER;
      case 0x007f: return _SC_TIMEOUTS;
      case 0x0080: return _SC_TRACE;
      case 0x0081: return _SC_TRACE_EVENT_FILTER;
      case 0x0082: return _SC_TRACE_EVENT_NAME_MAX;
      case 0x0083: return _SC_TRACE_INHERIT;
      case 0x0084: return _SC_TRACE_LOG;
      case 0x0085: return _SC_TRACE_NAME_MAX;
      case 0x0086: return _SC_TRACE_SYS_MAX;
      case 0x0087: return _SC_TRACE_USER_EVENT_MAX;
      case 0x0088: return _SC_TYPED_MEMORY_OBJECTS;
      case 0x0089: return _SC_V7_ILP32_OFF32;
      case 0x008a: return _SC_V7_ILP32_OFFBIG;
      case 0x008b: return _SC_V7_LP64_OFF64;
      case 0x008c: return _SC_V7_LPBIG_OFFBIG;
      case 0x008d: return _SC_XOPEN_STREAMS;
      // case 0x008e: return _SC_XOPEN_UUCP;
      case 0x008f: return _SC_LEVEL1_ICACHE_SIZE;
      case 0x0090: return _SC_LEVEL1_ICACHE_ASSOC;
      case 0x0091: return _SC_LEVEL1_ICACHE_LINESIZE;
      case 0x0092: return _SC_LEVEL1_DCACHE_SIZE;
      case 0x0093: return _SC_LEVEL1_DCACHE_ASSOC;
      case 0x0094: return _SC_LEVEL1_DCACHE_LINESIZE;
      case 0x0095: return _SC_LEVEL2_CACHE_SIZE;
      case 0x0096: return _SC_LEVEL2_CACHE_ASSOC;
      case 0x0097: return _SC_LEVEL2_CACHE_LINESIZE;
      case 0x0098: return _SC_LEVEL3_CACHE_SIZE;
      case 0x0099: return _SC_LEVEL3_CACHE_ASSOC;
      case 0x009a: return _SC_LEVEL3_CACHE_LINESIZE;
      case 0x009b: return _SC_LEVEL4_CACHE_SIZE;
      case 0x009c: return _SC_LEVEL4_CACHE_ASSOC;
      case 0x009d: return _SC_LEVEL4_CACHE_LINESIZE;
      default:
         assert(0 && "should not happend");
   }
   return 0xFFFF;
}

long
bionic_sysconf(int name)
{
   return sysconf(bionic_sysconf_to_glibc_sysconf(name));
}

// Debugging

int
strcmp(const char *s1, const char *s2)
{
   printf("%s == %s\n", s1, s2);
   return strcmp(s1, s2);
}

int
strncmp(const char *s1, const char *s2, size_t n)
{
   printf("%s == %s (%zu)\n", s1, s2, n);
   return strncmp(s1, s2, n);
}

ssize_t
readlink(const char *path, char *buf, size_t bufsize)
{
   printf("%s\n", path);
   return snprintf(buf, bufsize, "file.apk");
}

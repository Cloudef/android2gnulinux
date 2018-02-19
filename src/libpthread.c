#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <memory.h>
#include <pthread.h>
#include <assert.h>
#include <sys/mman.h>

typedef struct {
   union {
#if defined(__LP64__)
      int32_t __private[10];
#else
      int32_t __private[1];
#endif
      void *glibc;
   };
} bionic_mutex_t;

typedef struct {
   union {
      long __private;
      void *glibc;
   };
} bionic_mutexattr_t;

static const struct {
   bionic_mutex_t bionic;
   pthread_mutex_t glibc;
} bionic_mutex_init_map[] = {
   { .bionic = {{{ ((PTHREAD_MUTEX_NORMAL & 3) << 14) }}}, .glibc = PTHREAD_MUTEX_INITIALIZER },
   { .bionic = {{{ ((PTHREAD_MUTEX_RECURSIVE & 3) << 14) }}}, .glibc = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP },
   { .bionic = {{{ ((PTHREAD_MUTEX_ERRORCHECK & 3) << 14) }}}, .glibc = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP },
};

typedef struct {
   union {
#if defined(__LP64__)
      int32_t __private[12];
#else
      int32_t __private[1];
#endif
      void *glibc;
   };
} bionic_cond_t;

typedef struct {
   union {
      long __private;
      void *glibc;
   };
} bionic_condattr_t;

typedef int bionic_key_t;
_Static_assert(sizeof(bionic_key_t) == sizeof(pthread_key_t), "bionic_key_t and pthread_key_t size mismatch");

typedef int bionic_pthread_once_t;
_Static_assert(sizeof(bionic_pthread_once_t) == sizeof(pthread_once_t), "bionic_pthread_once_t and pthread_once_t size mismatch");

typedef long bionic_pthread_t;
_Static_assert(sizeof(bionic_pthread_t) == sizeof(pthread_t), "bionic_pthread_t and pthread_t size mismatch");

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define IS_MAPPED(x) is_mapped(x->glibc, sizeof(x))
#define INIT_IF_NOT_MAPPED(x, init) do { if (!IS_MAPPED(x)) init(x); } while(0)

static bool
is_mapped(void *mem, const size_t sz)
{
   const size_t ps = sysconf(_SC_PAGESIZE);
   assert(ps > 0);
   unsigned char vec[(sz + ps - 1) / ps];
   return !mincore(mem, sz, vec);
}

int
bionic_pthread_cond_timedwait_monotonic_np(bionic_cond_t *cond, bionic_mutex_t *mutex, const struct timespec *abstime)
{
   return 0;
}

int
bionic_pthread_mutexattr_settype(bionic_mutexattr_t *attr, int type)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_mutexattr_settype(attr->glibc, type);
}

int
bionic_pthread_mutexattr_destroy(bionic_mutexattr_t *attr)
{
   assert(attr);
   int ret = 0;
   if (IS_MAPPED(attr)) {
      ret = pthread_mutexattr_destroy(attr->glibc);
      munmap(attr->glibc, sizeof(pthread_mutexattr_t));
   }
   return ret;
}

int
bionic_pthread_mutexattr_init(bionic_mutexattr_t *attr)
{
   assert(attr);
   if (!IS_MAPPED(attr))
      attr->glibc = mmap(NULL, sizeof(pthread_mutexattr_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   return pthread_mutexattr_init(attr->glibc);
}

static void
default_pthread_mutex_init(bionic_mutex_t *mutex)
{
   assert(mutex && !IS_MAPPED(mutex));
   mutex->glibc = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

   for (size_t i = 0; i < ARRAY_SIZE(bionic_mutex_init_map); ++i) {
      if (!memcmp(&bionic_mutex_init_map[i].bionic, mutex, sizeof(*mutex)))
         continue;

      memcpy(mutex->glibc, &bionic_mutex_init_map[i].glibc, sizeof(bionic_mutex_init_map[i].glibc));
      return;
   }

   assert(0 && "no such default initializer???");
}

int
bionic_pthread_mutex_destroy(bionic_mutex_t *mutex)
{
   assert(mutex);
   int ret = 0;
   if (IS_MAPPED(mutex)) {
      ret = pthread_mutex_destroy(mutex->glibc);
      munmap(mutex->glibc, sizeof(pthread_mutex_t));
   }
   return ret;
}

int
bionic_pthread_mutex_init(bionic_mutex_t *mutex, const bionic_mutexattr_t *attr)
{
   assert(mutex && (!attr || IS_MAPPED(attr)));
   if (!IS_MAPPED(mutex))
      mutex->glibc = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   return pthread_mutex_init(mutex->glibc, (attr ? attr->glibc : NULL));
}

int
bionic_pthread_mutex_lock(bionic_mutex_t *mutex)
{
   assert(mutex);
   INIT_IF_NOT_MAPPED(mutex, default_pthread_mutex_init);
   return pthread_mutex_lock(mutex->glibc);
}

int
bionic_pthread_mutex_unlock(bionic_mutex_t *mutex)
{
   assert(mutex);
   INIT_IF_NOT_MAPPED(mutex, default_pthread_mutex_init);
   return pthread_mutex_unlock(mutex->glibc);
}

static void
default_pthread_cond_init(bionic_cond_t *cond)
{
   assert(cond && !IS_MAPPED(cond));
   cond->glibc = mmap(NULL, sizeof(pthread_cond_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   memset(cond->glibc, 0, sizeof(pthread_cond_t));
}

int
bionic_pthread_cond_destroy(bionic_cond_t *cond)
{
   assert(cond);
   int ret = 0;
   if (IS_MAPPED(cond)) {
      ret = pthread_cond_destroy(cond->glibc);
      munmap(cond->glibc, sizeof(pthread_cond_t));
   }
   return ret;
}

int
bionic_pthread_cond_init(bionic_cond_t *cond, const bionic_condattr_t *attr)
{
   assert(cond && (!attr || IS_MAPPED(attr)));
   if (!IS_MAPPED(cond))
      cond->glibc = mmap(NULL, sizeof(pthread_cond_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   return pthread_cond_init(cond->glibc, (attr ? attr->glibc : NULL));
}

int
bionic_pthread_cond_signal(bionic_cond_t *cond)
{
   assert(cond);
   INIT_IF_NOT_MAPPED(cond, default_pthread_cond_init);
   return pthread_cond_signal(cond->glibc);
}

int
bionic_pthread_cond_wait(bionic_cond_t *cond, bionic_mutex_t *mutex)
{
   assert(cond && mutex);
   INIT_IF_NOT_MAPPED(cond, default_pthread_cond_init);
   INIT_IF_NOT_MAPPED(mutex, default_pthread_mutex_init);
   return pthread_cond_wait(cond->glibc, mutex->glibc);
}

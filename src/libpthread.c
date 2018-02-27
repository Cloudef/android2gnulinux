#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <memory.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <sys/mman.h>

typedef struct {
   union {
      struct {
         unsigned int count;
#ifdef __LP64__
         int __reserved[3];
#endif
      } bionic;
      void *glibc;
   };
} bionic_sem_t;

typedef struct {
   union {
      struct {
         uint32_t flags;
         void* stack_base;
         size_t stack_size;
         size_t guard_size;
         int32_t sched_policy;
         int32_t sched_priority;
#ifdef __LP64__
         char __reserved[16];
#endif
      } bionic;
      void *glibc;
   };
} bionic_attr_t;

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

typedef int bionic_once_t;
_Static_assert(sizeof(bionic_once_t) == sizeof(pthread_once_t), "bionic_once_t and pthread_once_t size mismatch");

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

void
bionic___pthread_cleanup_push(void *c, void *routine, void *arg)
{
   assert(0 && "implement");
}

void
bionic___pthread_cleanup_pop(void *c, int execute)
{
   assert(0 && "implement");
}

int
bionic_pthread_cond_timedwait_relative_np(bionic_cond_t *cond, bionic_mutex_t *mutex, const struct timespec *reltime)
{
   assert(0 && "implement");
   return 0;
}

int
bionic_pthread_cond_timedwait_monotonic_np(bionic_cond_t *cond, bionic_mutex_t *mutex, const struct timespec *abstime)
{
   assert(0 && "implement");
   return 0;
}

int
bionic_sem_destroy(bionic_sem_t *sem)
{
   assert(sem);
   int ret = 0;
   if (IS_MAPPED(sem)) {
      ret = pthread_cond_destroy(sem->glibc);
      munmap(sem->glibc, sizeof(sem_t));
   }
   return ret;
}

int
bionic_sem_init(bionic_sem_t *sem, int pshared, unsigned int value)
{
   assert(sem);
   if (!IS_MAPPED(sem))
      sem->glibc = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   return sem_init(sem->glibc, pshared, value);
}

int
bionic_sem_post(bionic_sem_t *sem)
{
   assert(sem && IS_MAPPED(sem));
   return sem_post(sem->glibc);
}

int
bionic_sem_wait(bionic_sem_t *sem)
{
   assert(sem && IS_MAPPED(sem));
   return sem_wait(sem->glibc);
}

int
bionic_sem_trywait(bionic_sem_t *sem)
{
   assert(sem && IS_MAPPED(sem));
   return sem_trywait(sem->glibc);
}

int
bionic_sem_timedwait(bionic_sem_t *sem, const struct timespec *abs_timeout)
{
   assert(sem && IS_MAPPED(sem) && abs_timeout);
   return sem_timedwait(sem->glibc, abs_timeout);
}

int
bionic_pthread_attr_destroy(bionic_attr_t *attr)
{
   assert(attr);
   int ret = 0;
   if (IS_MAPPED(attr)) {
      ret = pthread_attr_destroy(attr->glibc);
      munmap(attr->glibc, sizeof(pthread_attr_t));
   }
   return ret;
}

int
bionic_pthread_attr_init(bionic_attr_t *attr)
{
   assert(attr);
   if (!IS_MAPPED(attr))
      attr->glibc = mmap(NULL, sizeof(pthread_attr_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   return pthread_attr_init(attr->glibc);
}

int
bionic_pthread_getattr_np(bionic_pthread_t thread, bionic_attr_t *attr)
{
   assert(thread && attr && !IS_MAPPED(attr));
   attr->glibc = mmap(NULL, sizeof(pthread_attr_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   return pthread_getattr_np((pthread_t*)thread, attr->glibc);
}

int
bionic_pthread_attr_settstack(bionic_attr_t *attr, void *stackaddr, size_t stacksize)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_setstack(attr->glibc, stackaddr, stacksize);
}

int
bionic_pthread_attr_getstack(const bionic_attr_t *attr, void *stackaddr, size_t *stacksize)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_getstack(attr->glibc, stackaddr, stacksize);
}

int
bionic_pthread_attr_setstacksize(bionic_attr_t *attr, size_t stacksize)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_setstacksize(attr->glibc, stacksize);
}

int
bionic_pthread_attr_getstacksize(const bionic_attr_t *attr, size_t *stacksize)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_getstacksize(attr->glibc, stacksize);
}

int
bionic_pthread_attr_setschedpolicy(bionic_attr_t *attr, int policy)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_setschedpolicy(attr->glibc, policy);
}

int
bionic_pthread_attr_getschedpolicy(bionic_attr_t *attr, int *policy)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_getschedpolicy(attr->glibc, policy);
}

int
bionic_pthread_attr_setschedparam(bionic_attr_t *attr, const struct sched_param *param)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_setschedparam(attr->glibc, param);
}

int
bionic_pthread_attr_getschedparam(bionic_attr_t *attr, struct sched_param *param)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_getschedparam(attr->glibc, param);
}

int
bionic_pthread_attr_setdetachstate(bionic_attr_t *attr, int detachstate)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_setdetachstate(attr->glibc, detachstate);
}

int
bionic_pthread_attr_getdetachstate(bionic_attr_t *attr, int *detachstate)
{
   assert(attr && IS_MAPPED(attr));
   return pthread_attr_getdetachstate(attr->glibc, detachstate);
}

int
bionic_pthread_create(bionic_pthread_t *thread, const bionic_attr_t *attr, void* (*start)(void*), void *arg)
{
   assert(thread && (!attr || IS_MAPPED(attr)));
   return pthread_create((pthread_t*)thread, (attr ? attr->glibc : NULL), start, arg);
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
bionic_pthread_mutex_trylock(bionic_mutex_t *mutex)
{
   assert(mutex);
   INIT_IF_NOT_MAPPED(mutex, default_pthread_mutex_init);
   return pthread_mutex_trylock(mutex->glibc);
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
bionic_pthread_cond_broadcast(bionic_cond_t *cond)
{
   assert(cond);
   INIT_IF_NOT_MAPPED(cond, default_pthread_cond_init);
   return pthread_cond_broadcast(cond->glibc);
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

int
bionic_pthread_cond_timedwait(bionic_cond_t *cond, bionic_mutex_t *mutex, const struct timespec *abs_timeout)
{
   assert(cond && mutex);
   INIT_IF_NOT_MAPPED(cond, default_pthread_cond_init);
   INIT_IF_NOT_MAPPED(mutex, default_pthread_mutex_init);
   return pthread_cond_timedwait(cond->glibc, mutex->glibc, abs_timeout);
}

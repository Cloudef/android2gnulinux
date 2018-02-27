#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#ifdef VERBOSE_FUNCTIONS
// Don't inline to get warnings from files that don't log at all
static void
#else
static inline void
#endif
verbose_log(const char *fmt, ...)
{
   static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutex_lock(&mutex);
   fprintf(stderr, "%lu: ", pthread_self());
   va_list ap;
   va_start(ap, fmt);
   vfprintf(stderr, fmt, ap);
   va_end(ap);
   fputc('\n', stderr);
   pthread_mutex_unlock(&mutex);
}

#ifdef VERBOSE_FUNCTIONS
#  define verbose verbose_log
#else
#  define verbose(...)
#endif

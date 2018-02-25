#pragma once

#ifdef VERBOSE_FUNCTIONS
#  include <stdio.h>
#  include <stdarg.h>
#  include <pthread.h>
static void
verbose(const char *fmt, ...)
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
#else
#  define verbose(...)
#endif

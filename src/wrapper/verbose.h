#pragma once

#ifdef VERBOSE_FUNCTIONS
#  include <stdarg.h>
static void
verbose(const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   vfprintf(stderr, fmt, ap);
   va_end(ap);
   fputc('\n', stderr);
}
#else
#  define verbose(...)
#endif

#pragma once

void
__attribute__((format(printf, 1, 2)))
verbose_log(const char *fmt, ...);

#ifdef VERBOSE_FUNCTIONS
#  define verbose verbose_log
#else
#  define verbose(...)
#endif

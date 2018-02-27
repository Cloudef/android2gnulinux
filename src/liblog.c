#include <stdarg.h>
#include <stdio.h>

int
__android_log_vprint(int prio, const char *tag,  const char *fmt, va_list ap)
{
   int ret = printf("(%d) %s :: ", prio, tag);
   ret += vprintf(fmt, ap);
   ret += (puts("") != EOF);
   return ret;
}

int
__android_log_print(int prio, const char *tag,  const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   int ret = __android_log_vprint(prio, tag, fmt, ap);
   va_end(ap);
   return ret;
}

int
__android_log_write(int prio, const char *tag, const char *text)
{
   return __android_log_print(prio, tag, "%s", text);
}

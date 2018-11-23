#include "wrapper.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include "verbose.h"
#include <pthread.h>

static __thread bool skip_trace;

void
verbose_log(const char *fmt, ...)
{
   if (skip_trace)
      return;

   static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutex_lock(&mutex);
   va_list ap;
   va_start(ap, fmt);
   static char buf[1024];
   vsnprintf(buf, sizeof(buf), fmt, ap);
   va_end(ap);
   fprintf(stderr, "%lu: %s\n", pthread_self(), buf);
   pthread_mutex_unlock(&mutex);
}

#ifdef VERBOSE_FUNCTIONS
#  ifdef ANDROID_X86_LINKER
__asm__(
   "wrapper_start: nop\n"
   "wrapper_store: push %edi\npush %esp\npush %ebp\npush %ebx\npush %eax\npush %ecx\npush %edx\n"
   "wrapper_symbol: pushl $0xFAFBFCFD\n" // arg1 for trace
   "wrapper_trace: .byte 0xE8, 0xFA, 0xFB, 0xFC, 0xFD\n" // CALL (trace)
   "wrapper_restore: pop %eax\npop %edx\npop %ecx\npop %eax\npop %ebx\npop %ebp\npop %esp\npop %edi\n"
   "wrapper_jmp: .byte 0xE9, 0xFA, 0xFB, 0xFC, 0xFD\n" // JMP
   "wrapper_ud: .byte 0x0F, 0xFF\n" // UD
   "wrapper_end: nop\n"
);
#     define WRAPPER_TRACE
#  else
#     warning "no wrapper asm for this platform, function tracing is not available"
#  endif
#endif

#ifdef WRAPPER_TRACE
extern unsigned char wrapper_start, wrapper_symbol, wrapper_trace, wrapper_restore, wrapper_jmp, wrapper_ud, wrapper_end;

static union {
   void *ptr;
   char* (*fun)(const char *mangled_name, char *output_buffer, size_t *length, int *status);
} __cxa_demangle;

static void
trace(const char *const symbol)
{
   assert(symbol);

   if (__cxa_demangle.ptr) {
      // >If output_buffer is not long enough, it is expanded using realloc
      // Holy fuck gcc what the fuck? Guess we don't use stack then, thanks
      int status;
      char *demangled;
      static __thread char *data;
      static __thread size_t size;

      // Avoid infinite recursion and tracing calls made by __cxa_demangle.fun
      if (skip_trace)
         return;

      skip_trace = true;
      demangled = __cxa_demangle.fun(symbol, data, &size, &status);
      skip_trace = false;

      if (demangled) {
         data = (data != demangled ? demangled : data);
         verbose("trace: %s", demangled);
         return;
      }
   }

   verbose("trace: %s", symbol);
}
#endif

void
wrapper_set_cpp_demangler(void *function)
{
#ifdef WRAPPER_TRACE
   __cxa_demangle.ptr = function;
   verbose_log("wrapper: set cpp_demangler to %p", function);
#endif
}

void*
wrapper_create(const char *const symbol, void *function)
{
   assert(symbol);

   if (!function) {
      verbose_log("FIXME: unimplemented symbol: %s", symbol);
      return NULL;
   }

#ifdef WRAPPER_TRACE
   static const union {
      void *ptr;
      void (*fun)(const char*);
   } tracefun = { .fun = trace };

   const size_t len = strlen(symbol) + 1;
   char *copy = malloc(len);
   assert(copy && "welp, malloc failed");
   memcpy(copy, symbol, len);
   const size_t sz = &wrapper_end - &wrapper_start;
   unsigned char *fun = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
   assert(fun != MAP_FAILED);
   memcpy(fun, &wrapper_start, sz);
#ifdef ANDROID_X86_LINKER
   memcpy(fun + (&wrapper_symbol - &wrapper_start) + 1, &copy, sizeof(copy));
   {
      const unsigned char *from = fun + (&wrapper_restore - &wrapper_start);
      const intptr_t to = (unsigned char*)tracefun.ptr - from;
      memcpy(fun + (&wrapper_trace - &wrapper_start) + 1, &to, sizeof(to));
   }{
      const unsigned char *from = fun + (&wrapper_ud - &wrapper_start);
      const intptr_t to = (unsigned char*)function - from;
      memcpy(fun + (&wrapper_jmp - &wrapper_start) + 1, &to, sizeof(to));
   }
#else
#   error "you forgot to implement the pointer setups for your asm platform"
#endif
   mprotect(fun, sz, PROT_READ | PROT_EXEC);
   return fun;
#else
   return function;
#endif
}

#include "wrapper.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <assert.h>
#include <sys/mman.h>

#ifdef ANDROID_X86_LINKER
__asm__(
    "wrapper_start: nop\n"
    "wrapper_symbol: pushl $0xFAFBFCFD\n"
    "wrapper_trace: movl $0xFAFBFCFD, %eax\ncall *%eax\npop %eax\n"
    "wrapper_call: movl $0xFAFBFCFD, %eax\njmp *%eax\n"
    "wrapper_end: nop\n"
);
#   define WRAPPER_TRACE
#else
#   warning "no wrapper asm for this platform, function tracing is not available"
#endif

#ifdef WRAPPER_TRACE
extern char wrapper_start, wrapper_symbol, wrapper_trace, wrapper_call, wrapper_end;
static char* (*__cxa_demangle)(const char *mangled_name, char *output_buffer, size_t *length, int *status);

static void
trace(const char *const symbol)
{
    if (__cxa_demangle) {
        // >If output_buffer is not long enough, it is expanded using realloc
        // Holy fuck gcc what the fuck? Guess we don't use stack then, thanks
        int status;
        char *demangled;
        if ((demangled = __cxa_demangle(symbol, NULL, NULL, &status))) {
            printf("trace: %s\n", demangled);
            free(demangled); // so pointless...
            return;
        }
    }

    printf("trace: %s\n", symbol);
}
#endif

void*
wrapper_create(const char *const symbol, void *function)
{
#ifdef WRAPPER_TRACE
    if (!__cxa_demangle)
        __cxa_demangle = dlsym(RTLD_DEFAULT, "__cxa_demangle");

    const size_t sz = &wrapper_end - &wrapper_start;
    unsigned char *fun = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(fun != MAP_FAILED);
    memcpy(fun, &wrapper_start, sz);
#ifdef ANDROID_X86_LINKER
    memcpy(fun + (&wrapper_symbol - &wrapper_start) + 1, &symbol, sizeof(symbol));
    memcpy(fun + (&wrapper_trace - &wrapper_start) + 1, (uintptr_t[]){ (uintptr_t)trace }, sizeof(uintptr_t));
    memcpy(fun + (&wrapper_call - &wrapper_start) + 1, &function, sizeof(function));
#else
#   error "should not happen"
#endif
    mprotect(fun, sz, PROT_READ | PROT_EXEC);
    return fun;
#else
    return function;
#endif
}

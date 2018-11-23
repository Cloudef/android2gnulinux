// generated with:
// grep '^#define _SC' | awk 'BEGIN { print "static inline int\nbionic_sysconf_to_glibc_sysconf(int name)\n{\n   switch (name) {" } { printf "#ifdef %s\n      case %s: return %s;\n#endif\n", $2, $3, $2 } END { print "      default:\n         fprintf(stderr, \"sysconf constant (%d) is not mapped\\n\", name);\n      abort();\n   }\n   return 0xFFFF;\n}" }'
// from bionic's bits/sysconf.h

static inline int
bionic_sysconf_to_glibc_sysconf(int name)
{
   switch (name) {
#ifdef _SC_ARG_MAX
      case 0x0000: return _SC_ARG_MAX;
#endif
#ifdef _SC_BC_BASE_MAX
      case 0x0001: return _SC_BC_BASE_MAX;
#endif
#ifdef _SC_BC_DIM_MAX
      case 0x0002: return _SC_BC_DIM_MAX;
#endif
#ifdef _SC_BC_SCALE_MAX
      case 0x0003: return _SC_BC_SCALE_MAX;
#endif
#ifdef _SC_BC_STRING_MAX
      case 0x0004: return _SC_BC_STRING_MAX;
#endif
#ifdef _SC_CHILD_MAX
      case 0x0005: return _SC_CHILD_MAX;
#endif
#ifdef _SC_CLK_TCK
      case 0x0006: return _SC_CLK_TCK;
#endif
#ifdef _SC_COLL_WEIGHTS_MAX
      case 0x0007: return _SC_COLL_WEIGHTS_MAX;
#endif
#ifdef _SC_EXPR_NEST_MAX
      case 0x0008: return _SC_EXPR_NEST_MAX;
#endif
#ifdef _SC_LINE_MAX
      case 0x0009: return _SC_LINE_MAX;
#endif
#ifdef _SC_NGROUPS_MAX
      case 0x000a: return _SC_NGROUPS_MAX;
#endif
#ifdef _SC_OPEN_MAX
      case 0x000b: return _SC_OPEN_MAX;
#endif
#ifdef _SC_PASS_MAX
      case 0x000c: return _SC_PASS_MAX;
#endif
#ifdef _SC_2_C_BIND
      case 0x000d: return _SC_2_C_BIND;
#endif
#ifdef _SC_2_C_DEV
      case 0x000e: return _SC_2_C_DEV;
#endif
#ifdef _SC_2_C_VERSION
      case 0x000f: return _SC_2_C_VERSION;
#endif
#ifdef _SC_2_CHAR_TERM
      case 0x0010: return _SC_2_CHAR_TERM;
#endif
#ifdef _SC_2_FORT_DEV
      case 0x0011: return _SC_2_FORT_DEV;
#endif
#ifdef _SC_2_FORT_RUN
      case 0x0012: return _SC_2_FORT_RUN;
#endif
#ifdef _SC_2_LOCALEDEF
      case 0x0013: return _SC_2_LOCALEDEF;
#endif
#ifdef _SC_2_SW_DEV
      case 0x0014: return _SC_2_SW_DEV;
#endif
#ifdef _SC_2_UPE
      case 0x0015: return _SC_2_UPE;
#endif
#ifdef _SC_2_VERSION
      case 0x0016: return _SC_2_VERSION;
#endif
#ifdef _SC_JOB_CONTROL
      case 0x0017: return _SC_JOB_CONTROL;
#endif
#ifdef _SC_SAVED_IDS
      case 0x0018: return _SC_SAVED_IDS;
#endif
#ifdef _SC_VERSION
      case 0x0019: return _SC_VERSION;
#endif
#ifdef _SC_RE_DUP_MAX
      case 0x001a: return _SC_RE_DUP_MAX;
#endif
#ifdef _SC_STREAM_MAX
      case 0x001b: return _SC_STREAM_MAX;
#endif
#ifdef _SC_TZNAME_MAX
      case 0x001c: return _SC_TZNAME_MAX;
#endif
#ifdef _SC_XOPEN_CRYPT
      case 0x001d: return _SC_XOPEN_CRYPT;
#endif
#ifdef _SC_XOPEN_ENH_I18N
      case 0x001e: return _SC_XOPEN_ENH_I18N;
#endif
#ifdef _SC_XOPEN_SHM
      case 0x001f: return _SC_XOPEN_SHM;
#endif
#ifdef _SC_XOPEN_VERSION
      case 0x0020: return _SC_XOPEN_VERSION;
#endif
#ifdef _SC_XOPEN_XCU_VERSION
      case 0x0021: return _SC_XOPEN_XCU_VERSION;
#endif
#ifdef _SC_XOPEN_REALTIME
      case 0x0022: return _SC_XOPEN_REALTIME;
#endif
#ifdef _SC_XOPEN_REALTIME_THREADS
      case 0x0023: return _SC_XOPEN_REALTIME_THREADS;
#endif
#ifdef _SC_XOPEN_LEGACY
      case 0x0024: return _SC_XOPEN_LEGACY;
#endif
#ifdef _SC_ATEXIT_MAX
      case 0x0025: return _SC_ATEXIT_MAX;
#endif
#ifdef _SC_IOV_MAX
      case 0x0026: return _SC_IOV_MAX;
#endif
#ifdef _SC_PAGESIZE
      case 0x0027: return _SC_PAGESIZE;
#endif
#ifdef _SC_PAGE_SIZE
      case 0x0028: return _SC_PAGE_SIZE;
#endif
#ifdef _SC_XOPEN_UNIX
      case 0x0029: return _SC_XOPEN_UNIX;
#endif
#ifdef _SC_XBS5_ILP32_OFF32
      case 0x002a: return _SC_XBS5_ILP32_OFF32;
#endif
#ifdef _SC_XBS5_ILP32_OFFBIG
      case 0x002b: return _SC_XBS5_ILP32_OFFBIG;
#endif
#ifdef _SC_XBS5_LP64_OFF64
      case 0x002c: return _SC_XBS5_LP64_OFF64;
#endif
#ifdef _SC_XBS5_LPBIG_OFFBIG
      case 0x002d: return _SC_XBS5_LPBIG_OFFBIG;
#endif
#ifdef _SC_AIO_LISTIO_MAX
      case 0x002e: return _SC_AIO_LISTIO_MAX;
#endif
#ifdef _SC_AIO_MAX
      case 0x002f: return _SC_AIO_MAX;
#endif
#ifdef _SC_AIO_PRIO_DELTA_MAX
      case 0x0030: return _SC_AIO_PRIO_DELTA_MAX;
#endif
#ifdef _SC_DELAYTIMER_MAX
      case 0x0031: return _SC_DELAYTIMER_MAX;
#endif
#ifdef _SC_MQ_OPEN_MAX
      case 0x0032: return _SC_MQ_OPEN_MAX;
#endif
#ifdef _SC_MQ_PRIO_MAX
      case 0x0033: return _SC_MQ_PRIO_MAX;
#endif
#ifdef _SC_RTSIG_MAX
      case 0x0034: return _SC_RTSIG_MAX;
#endif
#ifdef _SC_SEM_NSEMS_MAX
      case 0x0035: return _SC_SEM_NSEMS_MAX;
#endif
#ifdef _SC_SEM_VALUE_MAX
      case 0x0036: return _SC_SEM_VALUE_MAX;
#endif
#ifdef _SC_SIGQUEUE_MAX
      case 0x0037: return _SC_SIGQUEUE_MAX;
#endif
#ifdef _SC_TIMER_MAX
      case 0x0038: return _SC_TIMER_MAX;
#endif
#ifdef _SC_ASYNCHRONOUS_IO
      case 0x0039: return _SC_ASYNCHRONOUS_IO;
#endif
#ifdef _SC_FSYNC
      case 0x003a: return _SC_FSYNC;
#endif
#ifdef _SC_MAPPED_FILES
      case 0x003b: return _SC_MAPPED_FILES;
#endif
#ifdef _SC_MEMLOCK
      case 0x003c: return _SC_MEMLOCK;
#endif
#ifdef _SC_MEMLOCK_RANGE
      case 0x003d: return _SC_MEMLOCK_RANGE;
#endif
#ifdef _SC_MEMORY_PROTECTION
      case 0x003e: return _SC_MEMORY_PROTECTION;
#endif
#ifdef _SC_MESSAGE_PASSING
      case 0x003f: return _SC_MESSAGE_PASSING;
#endif
#ifdef _SC_PRIORITIZED_IO
      case 0x0040: return _SC_PRIORITIZED_IO;
#endif
#ifdef _SC_PRIORITY_SCHEDULING
      case 0x0041: return _SC_PRIORITY_SCHEDULING;
#endif
#ifdef _SC_REALTIME_SIGNALS
      case 0x0042: return _SC_REALTIME_SIGNALS;
#endif
#ifdef _SC_SEMAPHORES
      case 0x0043: return _SC_SEMAPHORES;
#endif
#ifdef _SC_SHARED_MEMORY_OBJECTS
      case 0x0044: return _SC_SHARED_MEMORY_OBJECTS;
#endif
#ifdef _SC_SYNCHRONIZED_IO
      case 0x0045: return _SC_SYNCHRONIZED_IO;
#endif
#ifdef _SC_TIMERS
      case 0x0046: return _SC_TIMERS;
#endif
#ifdef _SC_GETGR_R_SIZE_MAX
      case 0x0047: return _SC_GETGR_R_SIZE_MAX;
#endif
#ifdef _SC_GETPW_R_SIZE_MAX
      case 0x0048: return _SC_GETPW_R_SIZE_MAX;
#endif
#ifdef _SC_LOGIN_NAME_MAX
      case 0x0049: return _SC_LOGIN_NAME_MAX;
#endif
#ifdef _SC_THREAD_DESTRUCTOR_ITERATIONS
      case 0x004a: return _SC_THREAD_DESTRUCTOR_ITERATIONS;
#endif
#ifdef _SC_THREAD_KEYS_MAX
      case 0x004b: return _SC_THREAD_KEYS_MAX;
#endif
#ifdef _SC_THREAD_STACK_MIN
      case 0x004c: return _SC_THREAD_STACK_MIN;
#endif
#ifdef _SC_THREAD_THREADS_MAX
      case 0x004d: return _SC_THREAD_THREADS_MAX;
#endif
#ifdef _SC_TTY_NAME_MAX
      case 0x004e: return _SC_TTY_NAME_MAX;
#endif
#ifdef _SC_THREADS
      case 0x004f: return _SC_THREADS;
#endif
#ifdef _SC_THREAD_ATTR_STACKADDR
      case 0x0050: return _SC_THREAD_ATTR_STACKADDR;
#endif
#ifdef _SC_THREAD_ATTR_STACKSIZE
      case 0x0051: return _SC_THREAD_ATTR_STACKSIZE;
#endif
#ifdef _SC_THREAD_PRIORITY_SCHEDULING
      case 0x0052: return _SC_THREAD_PRIORITY_SCHEDULING;
#endif
#ifdef _SC_THREAD_PRIO_INHERIT
      case 0x0053: return _SC_THREAD_PRIO_INHERIT;
#endif
#ifdef _SC_THREAD_PRIO_PROTECT
      case 0x0054: return _SC_THREAD_PRIO_PROTECT;
#endif
#ifdef _SC_THREAD_SAFE_FUNCTIONS
      case 0x0055: return _SC_THREAD_SAFE_FUNCTIONS;
#endif
#ifdef _SC_NPROCESSORS_CONF
      case 0x0060: return _SC_NPROCESSORS_CONF;
#endif
#ifdef _SC_NPROCESSORS_ONLN
      case 0x0061: return _SC_NPROCESSORS_ONLN;
#endif
#ifdef _SC_PHYS_PAGES
      case 0x0062: return _SC_PHYS_PAGES;
#endif
#ifdef _SC_AVPHYS_PAGES
      case 0x0063: return _SC_AVPHYS_PAGES;
#endif
#ifdef _SC_MONOTONIC_CLOCK
      case 0x0064: return _SC_MONOTONIC_CLOCK;
#endif
#ifdef _SC_2_PBS
      case 0x0065: return _SC_2_PBS;
#endif
#ifdef _SC_2_PBS_ACCOUNTING
      case 0x0066: return _SC_2_PBS_ACCOUNTING;
#endif
#ifdef _SC_2_PBS_CHECKPOINT
      case 0x0067: return _SC_2_PBS_CHECKPOINT;
#endif
#ifdef _SC_2_PBS_LOCATE
      case 0x0068: return _SC_2_PBS_LOCATE;
#endif
#ifdef _SC_2_PBS_MESSAGE
      case 0x0069: return _SC_2_PBS_MESSAGE;
#endif
#ifdef _SC_2_PBS_TRACK
      case 0x006a: return _SC_2_PBS_TRACK;
#endif
#ifdef _SC_ADVISORY_INFO
      case 0x006b: return _SC_ADVISORY_INFO;
#endif
#ifdef _SC_BARRIERS
      case 0x006c: return _SC_BARRIERS;
#endif
#ifdef _SC_CLOCK_SELECTION
      case 0x006d: return _SC_CLOCK_SELECTION;
#endif
#ifdef _SC_CPUTIME
      case 0x006e: return _SC_CPUTIME;
#endif
#ifdef _SC_HOST_NAME_MAX
      case 0x006f: return _SC_HOST_NAME_MAX;
#endif
#ifdef _SC_IPV6
      case 0x0070: return _SC_IPV6;
#endif
#ifdef _SC_RAW_SOCKETS
      case 0x0071: return _SC_RAW_SOCKETS;
#endif
#ifdef _SC_READER_WRITER_LOCKS
      case 0x0072: return _SC_READER_WRITER_LOCKS;
#endif
#ifdef _SC_REGEXP
      case 0x0073: return _SC_REGEXP;
#endif
#ifdef _SC_SHELL
      case 0x0074: return _SC_SHELL;
#endif
#ifdef _SC_SPAWN
      case 0x0075: return _SC_SPAWN;
#endif
#ifdef _SC_SPIN_LOCKS
      case 0x0076: return _SC_SPIN_LOCKS;
#endif
#ifdef _SC_SPORADIC_SERVER
      case 0x0077: return _SC_SPORADIC_SERVER;
#endif
#ifdef _SC_SS_REPL_MAX
      case 0x0078: return _SC_SS_REPL_MAX;
#endif
#ifdef _SC_SYMLOOP_MAX
      case 0x0079: return _SC_SYMLOOP_MAX;
#endif
#ifdef _SC_THREAD_CPUTIME
      case 0x007a: return _SC_THREAD_CPUTIME;
#endif
#ifdef _SC_THREAD_PROCESS_SHARED
      case 0x007b: return _SC_THREAD_PROCESS_SHARED;
#endif
#ifdef _SC_THREAD_ROBUST_PRIO_INHERIT
      case 0x007c: return _SC_THREAD_ROBUST_PRIO_INHERIT;
#endif
#ifdef _SC_THREAD_ROBUST_PRIO_PROTECT
      case 0x007d: return _SC_THREAD_ROBUST_PRIO_PROTECT;
#endif
#ifdef _SC_THREAD_SPORADIC_SERVER
      case 0x007e: return _SC_THREAD_SPORADIC_SERVER;
#endif
#ifdef _SC_TIMEOUTS
      case 0x007f: return _SC_TIMEOUTS;
#endif
#ifdef _SC_TRACE
      case 0x0080: return _SC_TRACE;
#endif
#ifdef _SC_TRACE_EVENT_FILTER
      case 0x0081: return _SC_TRACE_EVENT_FILTER;
#endif
#ifdef _SC_TRACE_EVENT_NAME_MAX
      case 0x0082: return _SC_TRACE_EVENT_NAME_MAX;
#endif
#ifdef _SC_TRACE_INHERIT
      case 0x0083: return _SC_TRACE_INHERIT;
#endif
#ifdef _SC_TRACE_LOG
      case 0x0084: return _SC_TRACE_LOG;
#endif
#ifdef _SC_TRACE_NAME_MAX
      case 0x0085: return _SC_TRACE_NAME_MAX;
#endif
#ifdef _SC_TRACE_SYS_MAX
      case 0x0086: return _SC_TRACE_SYS_MAX;
#endif
#ifdef _SC_TRACE_USER_EVENT_MAX
      case 0x0087: return _SC_TRACE_USER_EVENT_MAX;
#endif
#ifdef _SC_TYPED_MEMORY_OBJECTS
      case 0x0088: return _SC_TYPED_MEMORY_OBJECTS;
#endif
#ifdef _SC_V7_ILP32_OFF32
      case 0x0089: return _SC_V7_ILP32_OFF32;
#endif
#ifdef _SC_V7_ILP32_OFFBIG
      case 0x008a: return _SC_V7_ILP32_OFFBIG;
#endif
#ifdef _SC_V7_LP64_OFF64
      case 0x008b: return _SC_V7_LP64_OFF64;
#endif
#ifdef _SC_V7_LPBIG_OFFBIG
      case 0x008c: return _SC_V7_LPBIG_OFFBIG;
#endif
#ifdef _SC_XOPEN_STREAMS
      case 0x008d: return _SC_XOPEN_STREAMS;
#endif
#ifdef _SC_XOPEN_UUCP
      case 0x008e: return _SC_XOPEN_UUCP;
#endif
#ifdef _SC_LEVEL1_ICACHE_SIZE
      case 0x008f: return _SC_LEVEL1_ICACHE_SIZE;
#endif
#ifdef _SC_LEVEL1_ICACHE_ASSOC
      case 0x0090: return _SC_LEVEL1_ICACHE_ASSOC;
#endif
#ifdef _SC_LEVEL1_ICACHE_LINESIZE
      case 0x0091: return _SC_LEVEL1_ICACHE_LINESIZE;
#endif
#ifdef _SC_LEVEL1_DCACHE_SIZE
      case 0x0092: return _SC_LEVEL1_DCACHE_SIZE;
#endif
#ifdef _SC_LEVEL1_DCACHE_ASSOC
      case 0x0093: return _SC_LEVEL1_DCACHE_ASSOC;
#endif
#ifdef _SC_LEVEL1_DCACHE_LINESIZE
      case 0x0094: return _SC_LEVEL1_DCACHE_LINESIZE;
#endif
#ifdef _SC_LEVEL2_CACHE_SIZE
      case 0x0095: return _SC_LEVEL2_CACHE_SIZE;
#endif
#ifdef _SC_LEVEL2_CACHE_ASSOC
      case 0x0096: return _SC_LEVEL2_CACHE_ASSOC;
#endif
#ifdef _SC_LEVEL2_CACHE_LINESIZE
      case 0x0097: return _SC_LEVEL2_CACHE_LINESIZE;
#endif
#ifdef _SC_LEVEL3_CACHE_SIZE
      case 0x0098: return _SC_LEVEL3_CACHE_SIZE;
#endif
#ifdef _SC_LEVEL3_CACHE_ASSOC
      case 0x0099: return _SC_LEVEL3_CACHE_ASSOC;
#endif
#ifdef _SC_LEVEL3_CACHE_LINESIZE
      case 0x009a: return _SC_LEVEL3_CACHE_LINESIZE;
#endif
#ifdef _SC_LEVEL4_CACHE_SIZE
      case 0x009b: return _SC_LEVEL4_CACHE_SIZE;
#endif
#ifdef _SC_LEVEL4_CACHE_ASSOC
      case 0x009c: return _SC_LEVEL4_CACHE_ASSOC;
#endif
#ifdef _SC_LEVEL4_CACHE_LINESIZE
      case 0x009d: return _SC_LEVEL4_CACHE_LINESIZE;
#endif
      default:
         fprintf(stderr, "sysconf constant (%d) is not mapped\n", name);
         abort();
   }
   return 0xFFFF;
}

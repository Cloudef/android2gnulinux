PREFIX ?= /usr/local
BINDIR ?= /bin

MAKEFLAGS += --no-builtin-rules

WARNINGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wstrict-aliasing=3 -Wstrict-overflow=3 -Wstack-usage=4096000 \
	-Wfloat-equal -Wcast-align -Wpointer-arith -Wchar-subscripts -Warray-bounds=2 -Wno-unused-parameter

override CFLAGS ?= -g -O2 $(WARNINGS)
override CFLAGS += -std=c11
override CPPFLAGS ?= -D_FORTIFY_SOURCE=2
override CPPFLAGS += -Isrc -DANDROID_X86_LINKER # -DVERBOSE_FUNCTIONS

bins = andre
all: $(bins)

# https://developer.android.com/ndk/guides/abis
# https://android.googlesource.com/platform/ndk/+/ics-mr0/docs/STANDALONE-TOOLCHAIN.html
# https://android.googlesource.com/platform/ndk/+/ics-mr0/docs/CPU-ARCH-ABIS.html
# you can also try compiling with your custom ABI with the all target,
# but compatibility with android binaries is not guaranteed

x86: override CFLAGS += -march=i686 -mtune=intel -mssse3 -mstackrealign -mfpmath=sse -m32
x86: override LDFLAGS += -march=i686 -m32
x86: all

x86_64: override CFLAGS += -march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel -fPIC
x86_64: override LDFLAGS += -march=x86-64 -m64
x86_64: all

armeabi: override CFLAGS += -march=armv5te -mthumb
armeabi: override LDFLAGS += -march=armv5te
armeabi: all

armeabi-v7a: override CFLAGS += -march=armv7-a -mfloat-abi=softfp -mthumb
armeabi-v7a: override LDFLAGS += -march=armv7-a -Wl,--fix-cortex-a8
armeabi-v7a: all
armeabi-v7a-neon: override CFLAGS += -mfpu=neon
armeabi-v7a-neon: armeabi-v7a

arm64-v8a: all

%.a:
	$(LINK.c) -c $(filter %.c,$^) -o $@

runtime:
	mkdir $@

runtime/%.so: | runtime
	$(LINK.c) -shared $(filter %.c %.a,$^) $(LDLIBS) -o $@

$(bins): %:
	$(LINK.c) $(filter %.c %.a,$^) $(LDLIBS) -o $@

wrapper.a: private override CPPFLAGS += -D_GNU_SOURCE
wrapper.a: private override CFLAGS += -fvisibility=hidden
wrapper.a: src/wrapper/verbose.h src/wrapper/wrapper.c src/wrapper/wrapper.h

runtime/libpthread.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libpthread.so: private override LDLIBS += -lpthread -lrt
runtime/libpthread.so: src/libpthread.c
runtime/libdl.so: private override CPPFLAGS += -D_GNU_SOURCE -DLINKER_DEBUG=1
runtime/libdl.so: private WARNINGS += -Wno-pedantic -Wno-variadic-macros -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
runtime/libdl.so: private override LDLIBS += -ldl -lpthread
runtime/libdl.so: wrapper.a src/linker/dlfcn.c src/linker/linker.c src/linker/linker_environ.c src/linker/rt.c src/linker/strlcpy.c
runtime/libc.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libc.so: private override LDFLAGS += -Wl,-wrap,_IO_file_xsputn
runtime/libc.so: private override CFLAGS += -Wno-deprecated-declarations
runtime/libc.so: private override LDLIBS += `pkg-config --libs libbsd libunwind`
runtime/libc.so: wrapper.a src/libc.c src/libc-stdio.c src/libc-sha1.c src/libc-antiantidebug.c
runtime/libc.so: src/libc-ctype.h src/libc-sysconf.h src/libc-verbose.h
runtime/libandroid.so: private override LDLIBS += `pkg-config --libs glfw3`
runtime/libandroid.so: src/libandroid.c
runtime/liblog.so: src/liblog.c
runtime/libEGL.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libEGL.so: private override LDLIBS += -lEGL `pkg-config --libs glfw3`
runtime/libEGL.so: src/libEGL.c
runtime/libOpenSLES.so: private WARNINGS += -Wno-pedantic
runtime/libOpenSLES.so: wrapper.a src/libOpenSLES.c

runtime/libjvm.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libjvm.so: private WARNINGS += -Wno-pedantic
runtime/libjvm.so: wrapper.a src/jvm/jvm.c
runtime/libjvm-java.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libjvm-java.so: src/libjvm-java.c
runtime/libjvm-android.so: src/libjvm-android.c
runtime/libjvm-unity.so: src/libjvm-unity.c
runtime/libjvm-misc.so: src/libjvm-misc.c

# trick linker to link against unversioned libs
libdl.so: runtime/libdl.so
	ln -s $< $@
libpthread.so: runtime/libpthread.so
	ln -s $< $@

andre: private override CPPFLAGS += -D_GNU_SOURCE
andre: private override LDFLAGS += -L. -Wl,-Y,runtime,-rpath,runtime
andre: private override LDLIBS += -ldl -lpthread
andre: private override LDLIBS += -ljvm -ljvm-java -ljvm-android -ljvm-unity -ljvm-misc
andre: src/loader.c libdl.so libpthread.so
andre: runtime/libpthread.so runtime/libc.so
andre: runtime/libandroid.so runtime/liblog.so
andre: runtime/libEGL.so runtime/libOpenSLES.so
andre: runtime/libjvm.so runtime/libjvm-java.so runtime/libjvm-android.so runtime/libjvm-unity.so runtime/libjvm-misc.so

install-bin: $(bins)
	install -Dm755 $^ -t "$(DESTDIR)$(PREFIX)$(BINDIR)"

install: install-bin

clean:
	$(RM) $(bins) *.a
	$(RM) -r runtime

.PHONY: all x86 x86_64 armeabi armeabi-v7a armeabi-v7a-neon arm64-v8a clean install
.INTERMEDIATE: libdl.so libpthread.so

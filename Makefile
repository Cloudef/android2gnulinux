PREFIX ?= /usr/local
BINDIR ?= /bin

MAKEFLAGS += --no-builtin-rules

# GCC 7: -Wstringop-overflow=, -Walloc-size-larger-than=, -Wduplicated-{branches,cond}
WARNINGS := -Wall -Wextra -Wpedantic -Wformat=2 -Wstrict-aliasing=3 -Wstrict-overflow=3 -Wstack-usage=4096000 \
	-Wfloat-equal -Wcast-align -Wpointer-arith -Wchar-subscripts -Warray-bounds=2 -Wno-unused-parameter

override CFLAGS ?= -g
override CFLAGS += -std=c11 $(WARNINGS)
override CPPFLAGS += -Isrc -DANDROID_X86_LINKER -DVERBOSE_FUNCTIONS

bins = app
all: $(bins)

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

runtime/libdl.so: private override CPPFLAGS += -D_GNU_SOURCE -DLINKER_DEBUG=1
runtime/libdl.so: private override CFLAGS += -Wno-pedantic -Wno-variadic-macros -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
runtime/libdl.so: private override LDLIBS += -ldl -lpthread
runtime/libdl.so: wrapper.a src/linker/dlfcn.c src/linker/linker.c src/linker/linker_environ.c src/linker/rt.c src/linker/strlcpy.c
runtime/libc.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libc.so: private override LDFLAGS += -Wl,-wrap,_IO_file_xsputn
runtime/libc.so: private override CFLAGS += -Wno-deprecated-declarations
runtime/libc.so: private override LDLIBS += `pkg-config --libs libbsd libunwind`
runtime/libc.so: wrapper.a src/libc.c src/libc-stdio.c src/libc-sha1.c src/libc-antiantidebug.c
runtime/libpthread.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libpthread.so: private override LDLIBS += -lpthread
runtime/libpthread.so: src/libpthread.c
runtime/libandroid.so: private override LDLIBS += `pkg-config --libs glfw3`
runtime/libandroid.so: src/libandroid.c
runtime/liblog.so: src/liblog.c
runtime/libEGL.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libEGL.so: private override LDLIBS += -lEGL `pkg-config --libs glfw3`
runtime/libEGL.so: src/libEGL.c
runtime/libOpenSLES.so: private override CFLAGS += -Wno-pedantic
runtime/libOpenSLES.so: wrapper.a src/libOpenSLES.c

runtime/libjvm.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libjvm.so: private override CFLAGS += -Wno-pedantic
runtime/libjvm.so: wrapper.a src/jvm/jvm.c
runtime/libjvm-java.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libjvm-java.so: src/libjvm-java.c
runtime/libjvm-android.so: src/libjvm-android.c
runtime/libjvm-unity.so: src/libjvm-unity.c

# trick linker to link against unversioned libs
libdl.so: runtime/libdl.so
	ln -s $< $@
libpthread.so: runtime/libpthread.so
	ln -s $< $@

app: private override CFLAGS += -D_DEFAULT_SOURCE
app: private override LDFLAGS += -L. -Wl,-Y,runtime,-rpath,runtime
app: private override LDLIBS += -ldl -lpthread
app: private override LDLIBS += -ljvm -ljvm-java -ljvm-android -ljvm-unity
app: src/app.c libdl.so libpthread.so
app: runtime/libpthread.so runtime/libc.so
app: runtime/libandroid.so runtime/liblog.so
app: runtime/libEGL.so runtime/libOpenSLES.so
app: runtime/libjvm.so runtime/libjvm-java.so runtime/libjvm-android.so runtime/libjvm-unity.so

install-bin: $(bins)
	install -Dm755 $^ -t "$(DESTDIR)$(PREFIX)$(BINDIR)"

install: install-bin

clean:
	$(RM) $(bins) *.a
	$(RM) -r runtime

.PHONY: all clean install
.INTERMEDIATE: libdl.so libpthread.so

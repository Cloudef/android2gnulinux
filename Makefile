PREFIX ?= /usr/local
BINDIR ?= /bin

MAKEFLAGS += --no-builtin-rules

# GCC 7: -Wstringop-overflow=, -Walloc-size-larger-than=, -Wduplicated-{branches,cond}
WARNINGS := -Wall -Wextra -Wpedantic -Wformat=2 -Wstrict-aliasing=3 -Wstrict-overflow=3 -Wstack-usage=12500 \
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

verbose: src/wrapper/verbose.h
wrapper.a: private override CPPFLAGS += -D_GNU_SOURCE
wrapper.a: verbose src/wrapper/wrapper.c src/wrapper/wrapper.h

runtime/libdl.so: private override CPPFLAGS += -D_GNU_SOURCE -DLINKER_DEBUG=1
runtime/libdl.so: private override CFLAGS += -Wno-pedantic -Wno-variadic-macros -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
runtime/libdl.so: wrapper.a src/linker/dlfcn.c src/linker/linker.c src/linker/linker_environ.c src/linker/rt.c src/linker/strlcpy.c
runtime/libc.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libc.so: private override LDFLAGS += -Wl,-wrap,_IO_file_xsputn
runtime/libc.so: private override CFLAGS += -Wno-deprecated-declarations
runtime/libc.so: private override LDLIBS += `pkg-config --libs libbsd libunwind`
runtime/libc.so: verbose src/libc.c src/libc-stdio.c
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
native: runtime/libdl.so runtime/libc.so runtime/libpthread.so runtime/libandroid.so runtime/liblog.so runtime/libEGL.so runtime/libOpenSLES.so

jvm.a: private override CPPFLAGS += -D_GNU_SOURCE
jvm.a: private override CFLAGS += -Wno-unused-variable -Wno-pedantic
jvm.a: wrapper.a src/jvm/jvm.c
runtime/libjvm-java.so: private override CPPFLAGS += -D_GNU_SOURCE
runtime/libjvm-java.so: verbose src/libjvm-java.c
runtime/libjvm-android.so: verbose src/libjvm-android.c
runtime/libjvm-unity.so: verbose src/libjvm-unity.c
java: runtime/libjvm-java.so runtime/libjvm-android.so runtime/libjvm-unity.so

app: private override CFLAGS += -Wno-pedantic -D_DEFAULT_SOURCE
app: private override LDLIBS += -ldl -Wl,-rpath,runtime runtime/libdl.so runtime/libpthread.so
app: private override LDLIBS += runtime/libjvm-java.so runtime/libjvm-android.so runtime/libjvm-unity.so
app: wrapper.a src/app.c native jvm.a java

install-bin: $(bins)
	install -Dm755 $^ -t "$(DESTDIR)$(PREFIX)$(BINDIR)"

install: install-bin

clean:
	$(RM) $(bins) *.a
	$(RM) -r runtime

.PHONY: all clean install

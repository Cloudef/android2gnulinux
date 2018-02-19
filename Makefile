PREFIX ?= /usr/local
BINDIR ?= /bin

MAKEFLAGS += --no-builtin-rules

# GCC 7: -Wstringop-overflow=, -Walloc-size-larger-than=, -Wduplicated-{branches,cond}
WARNINGS := -Wall -Wextra -Wpedantic -Wformat=2 -Wstrict-aliasing=3 -Wstrict-overflow=3 -Wstack-usage=12500 \
	-Wfloat-equal -Wcast-align -Wpointer-arith -Wchar-subscripts -Warray-bounds=2 -Wno-unused-parameter

override CFLAGS ?= -g
override CFLAGS += -std=c11 $(WARNINGS)
override CPPFLAGS += -Isrc

bins = app
all: $(bins) runtime/libc.so runtime/libandroid.so runtime/liblog.so

%.a:
	$(LINK.c) -c $(filter %.c,$^) -o $@

%.so:
	$(LINK.c) -shared $(filter %.c %.a,$^) $(LDLIBS) -o $@

$(bins): %:
	$(LINK.c) $(filter %.c %.a,$^) $(LDLIBS) -o $@

runtime:
	mkdir -p $@

wrapper.a: private CPPFLAGS += -D_GNU_SOURCE -DANDROID_X86_LINKER
wrapper.a: src/wrapper/wrapper.c
jvm.a: private CPPFLAGS += -D_GNU_SOURCE
jvm.a: private CFLAGS += -Wno-unused-variable -Wno-pedantic
jvm.a: wrapper.a src/jvm/jvm.c

runtime/libdl.so: private CPPFLAGS += -D_GNU_SOURCE -DANDROID_X86_LINKER -DLINKER_DEBUG=1
runtime/libdl.so: private CFLAGS += -Wno-pedantic -Wno-variadic-macros -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
runtime/libdl.so: runtime wrapper.a src/linker/dlfcn.c src/linker/linker.c src/linker/linker_environ.c src/linker/rt.c src/linker/strlcpy.c
runtime/libc.so: private CPPFLAGS += -D_GNU_SOURCE
runtime/libc.so: private LDLIBS += `pkg-config --libs libbsd`
runtime/libc.so: runtime src/libc.c
runtime/libpthread.so: private CPPFLAGS += -D_GNU_SOURCE
runtime/libpthread.so: private LDLIBS += -lpthread
runtime/libpthread.so: runtime src/libpthread.c
runtime/libandroid.so: private LDLIBS += `pkg-config --libs glfw3`
runtime/libandroid.so: runtime src/libandroid.c
runtime/liblog.so: runtime src/liblog.c
native: runtime/libdl.so runtime/libc.so runtime/libpthread.so runtime/libandroid.so runtime/liblog.so

runtime/libjvm-java.so: private CPPFLAGS += -D_GNU_SOURCE
runtime/libjvm-java.so: runtime src/libjvm-java.c
runtime/libjvm-android.so: runtime src/libjvm-android.c
java: runtime/libjvm-java.so runtime/libjvm-android.so

app: private LDLIBS += -ldl -Wl,-rpath,runtime runtime/libdl.so runtime/libpthread.so
app: private LDLIBS += runtime/libjvm-java.so runtime/libjvm-android.so
app: wrapper.a src/app.c native jvm.a java

install-bin: $(bins)
	install -Dm755 $^ -t "$(DESTDIR)$(PREFIX)$(BINDIR)"

install: install-bin

clean:
	$(RM) $(bins) *.a
	$(RM) -r runtime

.PHONY: all clean install

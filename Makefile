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

%.o: %.c
	$(COMPILE.c) $^ -o $@

%.so:
	$(LINK.c) -shared $(filter %.c,$^) $(LDLIBS) -o $@

%.a:
	$(AR) rvs $@ $^

$(bins): %:
	$(LINK.c) $(filter %.c %.a,$^) $(LDLIBS) -o $@

runtime:
	mkdir -p $@

runtime/libc.so: private CFLAGS += -D_GNU_SOURCE
runtime/libc.so: runtime src/libc.c
runtime/libpthread.so: private CFLAGS += -D_GNU_SOURCE
runtime/libpthread.so: private LDLIBS += -lpthread
runtime/libpthread.so: runtime src/libpthread.c
runtime/libandroid.so: runtime src/libandroid.c
runtime/liblog.so: runtime src/liblog.c

linker.a: CFLAGS += -D_GNU_SOURCE -DANDROID_X86_LINKER -DLINKER_DEBUG=1
linker.a: CFLAGS += -Wno-pedantic -Wno-variadic-macros -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
linker.a: src/linker/dlfcn.o src/linker/linker.o src/linker/linker_environ.o src/linker/rt.o src/linker/strlcpy.o
jvm.a: private CFLAGS += -Wno-unused-variable
jvm.a: src/fakejvm/jvm.o

app: private LDLIBS += -ldl -Wl,-rpath,runtime
app: src/app.c linker.a jvm.a runtime/libc.so runtime/libpthread.so runtime/libandroid.so runtime/liblog.so

install-bin: $(bins)
	install -Dm755 $^ -t "$(DESTDIR)$(PREFIX)$(BINDIR)"

install: install-bin

clean:
	$(RM) $(bins) *.a src/linker/*.o src/fakejvm/*.o
	$(RM) -r runtime

.PHONY: all clean install

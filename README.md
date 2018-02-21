# android2gnulinux

AOSP/Bionic compatiblity layer for GLIBC systems

## Notes

Currently heavy work in progress.
Linker will be rewritten as it's utter crap.
Need to add logging/tracing facility controlled from env.
That's about it, otherwise it's just implementing stuff.
`libjvm-*.so` implementations are not correct at all, but the point is to not make correct implementations yet,
just to get some games run, which are pretty forgiving for incorrect implementation :D.

## Building

```shell
make CFLAGS="-m32 -g" LDFLAGS="-m32"
```

Other than x86 is currently untested and probably doesn't work.

## Running

```shell
./app my-x86-android-lib.so
```

But currently `app` assumes pretty much `libunity.so`, so it will probably crash after returning from `JNI_OnLoad` :D.

## Notes about runtime libraries and porting

AOSP/bionic libs in `runtime` (the non `libjvm-*.so` ones) can be used to port android software to GNU/linux also.
The runtime libraries contain `bionic_` prefixed symbols for runtime compatibility only.
Functions that are not implemented in glibc, but are in AOSP/bionic are implemented normally in these libraries.
This means that the same libraries can be used for both, compiling and linking to GNU/linux android native code, and
running AOSP/bionic linked code. The PORTING and RUNTIME capability is not optional compile-time, nor do we separate
PORTING and RUNTIME libraries. This is for pure simplicity.

If you have to load/use bionic linked libraries from your ported application, you can use the `libdl.so`.
It has `bionic_` prefixed versions of the usual `dlfcn.h` functions.

If for some reason one wants minimal library for either PORTING or RUNTIME task, here are the rules you can apply for
symbol removal:

```
PORTING = remove all symbols with prefix `bionic_`, but not from `runtime/libdl.so` if you need it.
RUNTIME = remove symbol if it has `bionic_` prefixed version, and keep the `bionic_` version only.
```

That said, the libraries are pretty small anyways.

## JNI and JVM implementations

We implement the JNI interface and simulate a rather unusual JVM that only calls C functions.
Native android code that uses JNI, thinks it's talking to java but in reality it's calling functions from the
`runtime/libjvm-*.so` libraries. The `runtime/libjvm-*.so` libraries implement java apis referenced by the
application in C.

In future, there might be very small java bytecode interpreter, that we can use to eliminate the need of porting
internal java dependencies and implementing entrypoint for every native application. We would only have to implement
the external java apis that exist in android system image in C, and yes, this also means you could run android
java applications.

If you are porting android application to GNU/Linux that heavily relies on JNI, you can use the `libjvm.a` to
use the minimal JVM inside the application, without having to rewrite the JNI reliant code. At this moment however,
you still have to manually do the initial JNI calls that setup and initialize your application. This may not be
needed eventually, if we implemented what was described above (minimal java bytecode interpreter).

## Related Work

[apkenv](https://github.com/thp/apkenv.git)
This is what inspired this project. The current linker code originates from this project (modified from AOSP).
apkenv's in pretty messy state though and doesn't do clean implementation. It's still interesting approach and
you should check it out. Also helpful resource for this project.

[libhybris](https://github.com/libhybris/libhybris)
Very badly documented project, but I guess it has some overlap with this project?
Seems to be mainly aimed for using android GPU drivers on GNU/Linux and nothing else really.

## Q & A

### Why not use dalvikvm?

While we could use dalvikvm to handle the JNI and java, we still would have to reimplement all the android java apis
to be GNU/Linux compatible, or run android services in container or something. This is probably what `anbox` and such
projects do. The scope of this project is however something that does not need anything from android system. The
minimal bytecode interpreter would be fun research project to see how little code we can achieve all this with. Of
course java's standard library is quite large so the work for reimplementing java apis should not be underestimated.

Our minimal JNI/JVM implementation is clearly separated from other AOSP/bionic components however, so it's perfectly
possible for someone to use dalvikvm and android java libraries/services for the java bits instead. If someone's
gonna do and maintain that work and it doesn't feel too clunky/hacky/invasive I have nothing against such code in
this project. (aka, the JNI/JVM implementation is not hardcoded)

### How about arm libraries?

You can use qemu. For hw acceleration, we could use custom libGLES libs that communicate with host system. Though
such libraries may exist already, maybe even inside qemu project.. I haven't researched.

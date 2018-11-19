#!/bin/sh

argv0="$0"
msg() { printf -- '%s: %s\n' "${argv0##*/}" "$@" 1>&2; }
err() { msg "$@"; exit 1; }

[ -z "$1" ] && err 'usage: <apk>'
pkgfile="$(realpath "$1")"

arch="$(file ./andre | awk -F', ' '{print $2}')"
case "$arch" in
   'Intel 80386')
      arch="x86"
      ;;
   'ARM')
      arch="armeabi-v7a"
      ;;
   *)
      err "dont know how to convert from elf arch '$arch' to android arch"
      ;;
esac

pkgname="$(aapt2 dump --file AndroidManifest.xml "$1" | sed -nr 's/.*A: package="([^ "]+).*/\1/p')"
[ -z "$pkgname" ] && err "not a valid apk (missing package name)"

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT
unzip "$1" -d "$tmpdir"

export ANDROID_PACKAGE_FILE="$pkgfile"
export ANDROID_PACKAGE_CODE_PATH="$tmpdir"
export ANDROID_PACKAGE_NAME="$pkgname"

# TODO: when we have first release, make this follow XDG spec
export ANDROID_EXTERNAL_FILES_DIR="$PWD/local/data/$pkgname/files"
export ANDROID_EXTERNAL_OBB_DIR="$PWD/local/data/$pkgname/obb"

# XXX: We only work with unity stuff for now
./andre "$tmpdir/lib/$arch/libunity.so"

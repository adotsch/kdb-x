#!/bin/bash
set -e
set -x

# Runs on linux-64 for every target platform (zig cross-compiles), building
# only the binary for $TARGET_PLATFORM (set by recipe.yaml). Suffixes come
# from upstream's targets.mk.
case "$TARGET_PLATFORM" in
  linux-64)      T=init.li64.so ;;
  linux-aarch64) T=init.la64.so ;;
  osx-64)        T=init.mi64.so ;;
  osx-arm64)     T=init.ma64.so ;;
  win-64)        T=init.wi64.dll ;;
  *) echo "unsupported TARGET_PLATFORM '$TARGET_PLATFORM'" >&2; exit 1 ;;
esac

# common.mk's own `k.h:` rule curls it per-module as a make prerequisite —
# no need (and no source) to copy a root-level k.h first.
for m in bit defer flock udp selectk; do
  make -C $m $T
done

# $PREFIX/lib/q/mod is where kdbx's own modules install to
DEST=$PREFIX/lib/q/mod/dot
mkdir -p $DEST
cp init.q README.md LICENSE $DEST/
for m in bit defer flock udp selectk; do
  mkdir -p $DEST/$m
  cp $m/$T $DEST/$m/
  cp $m/README.md $DEST/$m/
done

.PHONY: all clean

ZIG_BUILD=zig build -DENTRY=$(ENTRY) -Doptimize=ReleaseFast --prefix zig-out-$@ --cache-dir .zig-cache-$@ --global-cache-dir .zig-cache-$@

all: all64

all64: $(TARGETS_64)
all32: $(TARGETS_32)

SOURCES=*.c $(wildcard *.h)

k.h:
	curl -L https://github.com/KxSystems/kdb/raw/master/c/c/k.h > $@

$(LI64): k.h $(SOURCES)
	$(ZIG_BUILD) -Dtarget=x86_64-linux.2.12
	cp zig-out-$@/lib/libinit.so $@
	patchelf --remove-needed libc.so $@

$(LI32): k.h $(SOURCES)
	$(ZIG_BUILD) -Dtarget=x86-linux.2.12
	cp zig-out-$@/lib/libinit.so $@
	patchelf --remove-needed libc.so $@

$(LA64): k.h $(SOURCES)
	$(ZIG_BUILD) -Dtarget=aarch64-linux.2.12
	cp zig-out-$@/lib/libinit.so $@
	patchelf --remove-needed libc.so $@

$(LA32): k.h $(SOURCES)
	$(ZIG_BUILD) -Dtarget=arm-linux-gnueabi
	cp zig-out-$@/lib/libinit.so $@
	patchelf --remove-needed libc.so $@

$(MI64): k.h $(SOURCES)
	$(ZIG_BUILD) -Dtarget=x86_64-macos
	cp zig-out-$@/lib/libinit.dylib $@

$(MA64): k.h $(SOURCES)
	$(ZIG_BUILD) -Dtarget=aarch64-macos
	cp zig-out-$@/lib/libinit.dylib $@

$(WI64): k.h $(SOURCES) libq_x64.a
	$(ZIG_BUILD) -Dtarget=x86_64-windows-gnu
	cp zig-out-$@/bin/init.dll $@

$(WA64): k.h $(SOURCES) libq_arm64.a
	$(ZIG_BUILD) -Dtarget=aarch64-windows-gnu
	cp zig-out-$@/bin/init.dll $@

$(WI32): k.h $(SOURCES) libq_x86.a
	$(ZIG_BUILD) -Dtarget=x86-windows-gnu
	cp zig-out-$@/bin/init.dll $@


libq_x64.a: ../q.def
	zig dlltool -m i386:x86-64 -d ../q.def -D q.exe -l libq_x64.a

libq_x86.a: ../q.def
	zig dlltool -m i386 -d ../q.def -D q.exe -l libq_x86.a

libq_arm64.a: ../q.def
	zig dlltool -m arm64 -d ../q.def -D q.exe -l libq_arm64.a

clean:
	rm -rf zig-out* .zig-cache* *.so *.dll *.a *.o
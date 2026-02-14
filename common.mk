CC = gcc
CFLAGS=-O3 -std=c11 -fvisibility=hidden -fPIC
#CFLAGS=-g -std=c11 -fPIC
LFLAGS=-s
M:=$(shell $(CC) -dumpmachine)
A:=$(if $(filter %-linux-gnu,$M),l)$(if $(findstring -apple-,$M),m)$(if $(filter %-mingw32 %-msvc,$M),w)
A:=$(if $A,$A$(if $(filter x86_64-%,$M),i64)$(if $(filter aarch64-% arm64-%,$M),a64))
$(if $A,,$(error couldn't determine the target platform: $(CC) -dumpmachine reports $M))

E=so
ifeq ($(A),wi64)
NM=x86_64-w64-mingw32-nm
DLLTOOL=x86_64-w64-mingw32-dlltool
LFLAGS+=-static-libgcc -Wl,--subsystem,windows
LA=libq.a
E=dll
endif

ifeq ($(filter m%64,$(A)),$(A))
CFLAGS+=-Wno-parentheses -Wno-pointer-sign
LFLAGS+=-undefined dynamic_lookup
endif

LIB=init.$A.$E

lib: $(LIB)

k.h:
	curl -L https://github.com/KxSystems/kdb/raw/master/c/c/k.h > $@

q.def: Makefile
	echo 'LIBRARY q.exe' > $@; echo EXPORTS >> $@
	curl -L https://github.com/KxSystems/kdb/blob/master/w32/q.lib?raw=true > q.lib
	$(NM) -p q.lib | egrep 'T _' | sed 's/0* T _//' >>$@
	echo vi >> $@; echo vk >> $@

libq.a: q.def
	$(DLLTOOL) -v -l $@ -d $<

clean:
	-rm k.h
	-rm *.o
	-rm *.so
	-rm *.dll libq.a q.def q.lib


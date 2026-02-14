#include "binop.c"      //and

#define BINOP OR
#define binop or
#define binopx0 orx0
#define binop0y or0y
#include "binop.c"

#define BINOP XOR
#define binop xor
#define binopx0 xorx0
#define binop0y xor0y
#include "binop.c"

#define CNTUU 1
#include "bitfni.c"     //popc
#undef CNTUU

#define NOUU 1

#define bitfni      clz
#define BITFN(x)    _Generic((x), default: (x==0?sizeof(x)*8:__builtin_clz(x)-8*(sizeof(int)-sizeof(x))), J: (x==0?sizeof(x)*8:__builtin_clzll(x)))
#include "bitfni.c"

#define bitfni      ctz
#define BITFN(x)    _Generic((x), default: (x==0?sizeof(x)*8:__builtin_ctz(x)), J: (x==0?sizeof(x)*8:__builtin_ctzll(x)))
#include "bitfni.c"

#define bitfni      clrsb
#define BITFN(x)    _Generic((x), default: (__builtin_clrsb(x)-8*(sizeof(int)-sizeof(x))), J: __builtin_clrsbll(x))
#include "bitfni.c"

#include "not.c"

__attribute__((visibility("default")))
K1(kexport)
{
    R k(0,"{`not`and`or`xor`bitcount`clz`ctz`clrsb!x}",knk(8,dl(not,1),dl(and,2),dl(or,2),dl(xor,2),dl(popc,1),dl(clz,1),dl(ctz,1),dl(clrsb,1)),0);
}
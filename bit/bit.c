#include "binop.c"      //and

#define BINOP OR
#define binop bitor
#define binopx0 bitorx0
#define binop0y bitor0y
#include "binop.c"

#define BINOP XOR
#define binop bitxor
#define binopx0 bitxorx0
#define binop0y bitxor0y
#include "binop.c"


#define CNTUU 1
#include "bitfni.c"     //popc
#undef CNTUU


#define LZUU 1
#define bitfni      leadz
#define BITFN(x)    _Generic((x), default: (x==0?sizeof(x)*8:__builtin_clz(x)-8*(sizeof(int)-sizeof(x))), J: (x==0?sizeof(x)*8:__builtin_clzll(x)))
#include "bitfni.c"
#undef LZUU

#define TZUU 1
#define bitfni      trailz
#define BITFN(x)    _Generic((x), default: (x==0?sizeof(x)*8:__builtin_ctz(x)), J: (x==0?sizeof(x)*8:__builtin_ctzll(x)))
#include "bitfni.c"
#undef TZUU

#include "not.c"

__attribute__((visibility("default")))
K1(kexport)
{
    R k(0,"`bitnot`bitand`bitor`bitxor`bitcount`leadz`trailz!",knk(7,dl(bitnot,1),dl(bitand,2),dl(bitor,2),dl(bitxor,2),dl(bitcount,1),dl(leadz,1),dl(trailz,1)),0);
}
#include "bit.h"

#ifndef bitfni

#define bitfni   popc
#define BITFN(x) _Generic((x), default: __builtin_popcount(x), J: __builtin_popcountll(x))
#endif

K bitfni(K x)
{
    switch(xt)
    {
        case -KG: return ki(BITFN(xg));
        case -KH: return ki(BITFN(xh));
        case -KI: return ki(BITFN(xi));
        case -KJ: return ki(BITFN(xj));
        #ifdef CNTUU
        case -UU: return ki(BITFN(xJ[0])+BITFN(xJ[1]));
        #endif
        #ifdef LZUU
        case -UU: {I c0 = BITFN(bswap64(xJ[0])); return ki(c0<64?c0:64+BITFN(bswap64(xJ[1])));}
        #endif
        #ifdef TZUU
        case -UU: {I c1 = BITFN(bswap64(xJ[1])); return ki(c1<64?c1:64+BITFN(bswap64(xJ[0])));}
        #endif
        case  KG: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xG[i])); return r;}
        case  KH: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xH[i])); return r;}
        case  KI: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xI[i])); return r;}
        case  KJ: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xJ[i])); return r;}
        #ifdef CNTUU
        case UU: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xJ[2*i])+BITFN(xJ[2*i+1])); return r;}
        #endif
        #ifdef LZUU
        case UU: {K r=ktn(KI,xn); DO(xn,{I c0=BITFN(bswap64(xJ[2*i]));kI(r)[i]=c0<64?c0:64+BITFN(bswap64(xJ[2*i+1]));}); return r;}
        #endif
        #ifdef TZUU
        case UU: {K r=ktn(KI,xn); DO(xn,{I c1=BITFN(bswap64(xJ[2*i+1]));kI(r)[i]=c1<64?c1:64+BITFN(bswap64(xJ[2*i]));}); return r;}
        #endif
        case 0:
        {
            K r = ktn(0,xn);
            for(J N=xn,i=0;i<N;i++)
                if(!(kK(r)[i]=bitfni(xK[i]))) return r->n=i,r0(r),(K)0;
            return vk(r);
        }
        case 99:
        {
            K fxy = bitfni(xy);
            return fxy ? xD(r1(xx),fxy) : fxy;
        }
        case 98:
        {
            K ft = bitfni(x->k);
            return ft ? xT(ft) : ft;
        }
    }
    return krr("type");
}

#undef bitfni
#undef BITFN
#undef BITFNLL
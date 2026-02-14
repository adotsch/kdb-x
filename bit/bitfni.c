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
        case  KG: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xG[i])); return r;}
        case  KH: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xH[i])); return r;}
        case  KI: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xI[i])); return r;}
        case  KJ: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xJ[i])); return r;}
        #ifdef CNTUU
        case  UU: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=BITFN(xJ[2*i])+BITFN(xJ[2*i+1])); return r;}
        #endif
        case 0:
        {
            K r = ktn(0,xn);
            for(J N=xn,i=0;i<N;i++)
                if(!(kK(r)[i]=bitfni(xK[i]))) return r->n=i,r0(r),(K)0;
            return vk(r);
        }
    }
    return krr("type");
}

#undef bitfni
#undef BITFN
#undef BITFNLL
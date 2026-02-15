#include "bit.h"

K not(K x)
{
    switch(xt)
    {
        case -KG: return kg(~xg);
        case -KH: return kh(~xh);
        case -KI: return ki(~xi);
        case -KJ: return kj(~xj);
        case -UU: {K r=ka(-UU); kJ(r)[0]=~xJ[0]; kJ(r)[1]=~xJ[1]; return r;}
        case  KG: {K r=ktn(KG,xn); DO(xn,kG(r)[i]=~xG[i]); return r;}
        case  KH: {K r=ktn(KH,xn); DO(xn,kH(r)[i]=~xH[i]); return r;}
        case  KI: {K r=ktn(KI,xn); DO(xn,kI(r)[i]=~xI[i]); return r;}
        case  KJ: {K r=ktn(KJ,xn); DO(xn,kJ(r)[i]=~xJ[i]); return r;}
        case  UU: {K r=ktn(KJ,xn); DO(2*xn,kJ(r)[i]=~xJ[i]); return r;}
        case 0:
        {
            K r = ktn(0,xn);
            for(J N=xn,i=0;i<N;i++)
                if(!(kK(r)[i]=not(xK[i]))) return r->n=i,r0(r),(K)0;
            return r;
        }
        case 99:
        {
            K nxy = not(xy);
            return nxy ? xD(r1(xx),nxy) : nxy;
        }
        case 98:
        {
            K nt = not(x->k);
            return nt ? xT(nt) : nt;
        }
    }
    return krr("type");
}

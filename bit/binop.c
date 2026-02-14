#include "bit.h"

#ifndef BINOP

#define BINOP   AND
#define binop   and
#define binopx0 andx0
#define binop0y and0y

#endif


Z K binop(K x,K y);

Z K binopx0(K x,K y)
{
    K r = ktn(0,y->n);
    for(J N=y->n,i=0;i<N;i++)
        if(!(kK(r)[i]=binop(x,kK(y)[i]))) return r->n=i,r0(r),(K)0;
    return r;
}

inline Z K binop0y(K x, K y)
{
    SAMELEN;
    K r = ktn(0,xn);
    struct k0 y_i = {.t=-y->t};
    switch(y->t)
    {
        case 0:
            for(J N=xn,i=0;i<N;i++)
                if(!(kK(r)[i]=binop(xK[i],kK(y)[i]))) return r->n=i,r0(r),(K)0;         
        break;
        case KG:
            for(J N=xn,i=0;i<N;i++)
            {
                y_i.g = kG(y)[i];
                if(!(kK(r)[i]=binop(xK[i],&y_i))) return r->n=i,r0(r),(K)0;             
            }
        break;
        case KH: 
            for(J N=xn,i=0;i<N;i++)
            {
                y_i.h = kH(y)[i];
                if(!(kK(r)[i]=binop(xK[i],&y_i))) return r->n=i,r0(r),(K)0;             
            }
        break;
        case KI: 
            for(J N=xn,i=0;i<N;i++)
            {
                y_i.i = kI(y)[i];
                if(!(kK(r)[i]=binop(xK[i],&y_i))) return r->n=i,r0(r),(K)0;             
            }
        break;
        case KJ: 
            for(J N=xn,i=0;i<N;i++)
            {
                y_i.j = kJ(y)[i];
                if(!(kK(r)[i]=binop(xK[i],&y_i))) return r->n=i,r0(r),(K)0;             
            }
        break;
        case UU: 
            for(J N=xn,i=0;i<N;i++)
            {
                kU(&y_i)[0] = kU(y)[i];
                if(!(kK(r)[i]=binop(xK[i],&y_i))) return r->n=i,r0(r),(K)0;             
            }
        break;
    }
    return r;
}

Z K2(binop)
{
    if(xt>y->t) return binop(y,x);
    switch(xt)
    {
        case -KG:
            if(SAMET) return kg(xg BINOP y->g);
            if(SAMEV) {K r=ktn(y->t,y->n); DO(y->n,kG(r)[i]=xg BINOP kG(y)[i]); return r;}
            if(GENEY) return binopx0(x,y);
        break;
        case -KH:
            if(SAMET) return kh(xh BINOP y->h);
            if(SAMEV) {K r=ktn(y->t,y->n); DO(y->n,kH(r)[i]=xh BINOP kH(y)[i]); return r;}
            if(GENEY) return binopx0(x,y);
        break;
        case -KI:
            if(SAMET) return ki(xi BINOP y->i);
            if(SAMEV) {K r=ktn(y->t,y->n); DO(y->n,kI(r)[i]=xi BINOP kI(y)[i]); return r;}
            if(GENEY) return binopx0(x,y);
        break;
        case -KJ:
            if(SAMET) return kj(xj BINOP y->j);
            if(SAMEV) {K r=ktn(y->t,y->n); DO(y->n,kJ(r)[i]=xj BINOP kJ(y)[i]); return r;}
            if(GENEY) return binopx0(x,y);
        break;
        case -UU:
            if(SAMET) {K r=ka(-UU); kJ(r)[0] = xJ[0] BINOP kJ(y)[0]; kJ(r)[1] = xJ[1] BINOP kJ(y)[1]; return r; };
            if(SAMEV) {K r=ktn(y->t,y->n); DO(2*y->n,kJ(r)[i]=xj BINOP kJ(y)[i]); return r;}
            if(GENEY) return binopx0(x,y);
        break;
        case 0:
            switch(y->t)
            {
                case 0: case KG: case KH: case KI:case KJ:
                    return binop0y(x,y);
            }
        break;
        case KG:
            if(SAMET) {SAMELEN; K r=ktn(xt,xn); DO(xn,kG(r)[i]=xG[i] BINOP kG(y)[i]); return r;}
        break;
        case KH:
            if(SAMET) {SAMELEN; K r=ktn(xt,xn); DO(xn,kH(r)[i]=xH[i] BINOP kH(y)[i]); return r;}
        break;
        case KI:
            if(SAMET) {SAMELEN; K r=ktn(xt,xn); DO(xn,kI(r)[i]=xI[i] BINOP kI(y)[i]); return r;}
        break;
        case KJ:
            if(SAMET) {SAMELEN; K r=ktn(xt,xn); DO(xn,kJ(r)[i]=xJ[i] BINOP kJ(y)[i]); return r;}
        break;
        case UU:
            if(SAMET) {SAMELEN; K r=ktn(xt,xn); DO(2*xn,kJ(r)[i]=xJ[i] BINOP kJ(y)[i]); return r;}
        break;
    }
    return krr("nyi");
}

#undef binop
#undef binopx0
#undef binop0y
#undef BINOP
#include "k.h"

K1(destructor)
{
    r0(k(0,"get",xy,0));    //execute deferred code
    R 0;
}

I is_defer(K x) {R xt==112 && xn==2 && xx==destructor;}

K1(defer)
{
    if(xt<0)
        R krr("type");
    else if(is_defer(x))    //disable
    {
        K r = xy;
        xy=knk(0);
        R r;
    }
    else                    //create foreign, wrap x
    {
        x=knk(2,destructor,r1(x));
        xt=112;
        R x;
    }
}

__attribute__((visibility("default")))
K1(kexport)
{
    R k(0,"(1#`defer)!",knk(1,dl(defer,1)),0);
}
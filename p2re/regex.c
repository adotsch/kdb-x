#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define KXVER 3
#include "k.h"

//K destructor for regex_t* foreign objects
Z K1(rfree)
{
	regfree((V*)xy);free(xy);
	xn=0;	//prevent r0-ing
	R x;
};

Z int is_regex(K x)
{
	return xt==112 && xx==(K)rfree;
}

Z K1(comp)
{
	int cflags = REG_EXTENDED;
	if(!xt)
	{
		if((2==xn) && ((xy->t==-KS) || (xy->t==KS)))
		{
			K v = k(0,"`ext`icase`nl in",r1(xy),0);
			cflags = kG(v)[0]*REG_EXTENDED + kG(v)[1]*REG_ICASE + kG(v)[2]*REG_NEWLINE;
			r0(v);
			x = xx;
		}
		else
			R krr("type");
	} 
	if(xt!=KC) R krr("type");
	C str[xn+1];memcpy(str,xC,xn);str[xn]=0;
	V* r = malloc(sizeof(regex_t));
	if(regcomp(r,str,cflags))
		R free(r),krr("regcomp");
	K p = knk(2,(K)rfree,r); p->t=112;	//a regex_t* wrapped in a K object w/ its destructor
	R p;
}

Z K2(match)
{
	if(99==xt) {K r=match(xy,y);R r?xD(r1(xx),r):0;}	//dict
	if(0==xt) DO(xn,if(xK[i]->t!=KC) R krr("type"));	//chech types
	y = is_regex(y) ? r1(y) : comp(y);					//compile
	if(!y) R 0;	//error
	K ret = 0;
	V* re = kK(y)[1];
	#define MATCH(s) (!regexec(re,s,0,0,0))
	switch(xt)
	{
		case -KS:
		{
			ret = kb(MATCH(x->s));
			break;
		}
		case KC:
		{
			C str[xn+1];memcpy(str,xC,xn);str[xn]=0;
			ret = kb(MATCH(str));
			break;
		}
		case KS:
		{
			ret = ktn(KB,xn);
			DO(xn,kG(ret)[i]=MATCH(xS[i]));
			break;
		}
		case -76 ... -20:
		case 20 ... 76:
		{
			K x0 = k(0,"get",r1(x),0);
			ret = match(x0,y);
			r0(x0);
			break;
		}
		case 77:
		case 87:
		{
			K x0 = k(0,"@[;::;::]",r1(x),0);
			ret = match(x0,y);
			r0(x0);
			break;
		}
		case 0:
		{
			ret = ktn(KB,xn);
			for(J i=0;i<xn;i++)
			{
				J l=xK[i]->n;
				C str[l+1];memcpy(str,kC(xK[i]),l);str[l]=0;
				kG(ret)[i]=MATCH(str);
			}
			break;
		}
		default:
		{
			ret = krr("type");
		}
	}
	r0(y);
	R ret;
}

Z K matches_impl(K x,K y,G se)
{
	if((xt!=-KS)&&(xt!=KC)) R krr("type");	
	y = (y->t==112) ? r1(y) : comp(y);
	if(!y) R 0;	//error
	S str;C _str[(KC==xt)?xn+1:0];
	if(KC==xt)
	{
		str=_str;memcpy(_str,xC,xn);str[xn]=0;
	}
	else
		str=xs;
	K ret = 0;
	V* re = kK(y)[1];
	regmatch_t m[256];
	if(!regexec(re,str,256,m,0))
	{
		I n=0;while((n<256)&&(m[n].rm_so!=-1))n++;
		ret=ktn(0,n);
		if(se)
		{
			for(I i=0;i<n;i++)
			{
				K ri=ktn(KJ,2);
				kJ(ri)[0]=m[i].rm_so;kJ(ri)[1]=m[i].rm_eo;
				kK(ret)[i] = ri;
			}
		}
		else
		{
			for(I i=0;i<n;i++)
			{
				K ri=ktn(KC,m[i].rm_eo - m[i].rm_so);
				memcpy(kC(ri),str+m[i].rm_so,ri->n);
				kK(ret)[i] = ri;
			}
		}
	}
	else
		ret = knk(0);
	r0(y);
	R ret;
}

Z K2(matches)  {R matches_impl(x,y,0);}
Z K2(imatches) {R matches_impl(x,y,1);}

__attribute__((visibility("default")))
K1(kexport) {R k(0,"`re`rmatch`rmatches`irmatches!",knk(4,dl(comp,1),dl(match,2),dl(matches,2),dl(imatches,2)),0);}
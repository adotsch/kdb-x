/**
 * @file selectk.c
 * @author András Dőtsch (dotsch@gmail.com)
 * @brief KDB-X module for a select_k impl
 * 
 * @copyright Copyright (c) 2025
 */

#define T G
#define REL lt
#define select_k select_k_lt_G
#define select_k0 select_k0_lt_G
#include "selectk_impl.c"
#define T G
#define REL st
#define select_k select_k_st_G
#define select_k0 select_k0_st_G
#include "selectk_impl.c"
#define T H
#define REL lt
#define select_k select_k_lt_H
#define select_k0 select_k0_lt_H
#include "selectk_impl.c"
#define T H
#define REL st
#define select_k select_k_st_H
#define select_k0 select_k0_st_H
#include "selectk_impl.c"
#define T I
#define REL lt
#define select_k select_k_lt_I
#define select_k0 select_k0_lt_I
#include "selectk_impl.c"
#define T I
#define REL st
#define select_k select_k_st_I
#define select_k0 select_k0_st_I
#include "selectk_impl.c"
#define T J
#define REL lt
#define select_k select_k_lt_J
#define select_k0 select_k0_lt_J
#include "selectk_impl.c"
#define T J
#define REL st
#define select_k select_k_st_J
#define select_k0 select_k0_st_J
#include "selectk_impl.c"
#define T E
#define REL lt
#define select_k select_k_lt_E
#define select_k0 select_k0_lt_E
#include "selectk_impl.c"
#define T E
#define REL st
#define select_k select_k_st_E
#define select_k0 select_k0_st_E
#include "selectk_impl.c"
#define T F
#define REL lt
#define select_k select_k_lt_F
#define select_k0 select_k0_lt_F
#include "selectk_impl.c"
#define T F
#define REL st
#define select_k select_k_st_F
#define select_k0 select_k0_st_F
#include "selectk_impl.c"


Z I valid_x(K x) { return ((KG<=xt)&&(xt<=KC))||((KP<=xt)&&(xt<=KT));}

Z void x_select_k(K x, J* index, J k, I lt)
{
	if(k>SMALL_K)
	{
		for(J i=0;i<xn;i++) index[i]=i;
		switch(xt)
		{
			case KG: case KC:
				(lt?select_k_lt_G:select_k_st_G)(xG,index,0,xn-1,k-1); break;
			case KH: 
				(lt?select_k_lt_H:select_k_st_H)(xH,index,0,xn-1,k-1); break;
			case KI: case KM: case KD: case KU: case KV: case KT: 
				(lt?select_k_lt_I:select_k_st_I)(xI,index,0,xn-1,k-1); break;
			case KJ: case KP: case KN:
				(lt?select_k_lt_J:select_k_st_J)(xJ,index,0,xn-1,k-1); break;
			case KE:
				(lt?select_k_lt_E:select_k_st_E)(xE,index,0,xn-1,k-1); break;
			case KF: case KZ:
				(lt?select_k_lt_F:select_k_st_F)(xF,index,0,xn-1,k-1); break;
		}
	}
	else
	{
		for(J i=0;i<k;i++) index[i]=i;
		switch(xt)
		{
			case KG: case KC:
				(lt?select_k0_lt_G:select_k0_st_G)(xG,index,xn,k); break;
			case KH: 
				(lt?select_k0_lt_H:select_k0_st_H)(xH,index,xn,k); break;
			case KI: case KM: case KD: case KU: case KV: case KT: 
				(lt?select_k0_lt_I:select_k0_st_I)(xI,index,xn,k); break;
			case KJ: case KP: case KN:
				(lt?select_k0_lt_J:select_k0_st_J)(xJ,index,xn,k); break;
			case KE:
				(lt?select_k0_lt_E:select_k0_st_E)(xE,index,xn,k); break;
			case KF: case KZ:
				(lt?select_k0_lt_F:select_k0_st_F)(xF,index,xn,k); break;
		}
	}
}

Z K selectkv(K _n, K x, I lt, C ret)
{
	if(!valid_x(x)) R krr("array type");
	if(_n->t != -KJ) R krr("k type");
	J n = _n->j;
	if(n<1) return krr("k range");
	if(n>xn) n = xn;
	K is = ktn(KJ,n>SMALL_K?xn:n);
	x_select_k(x,kJ(is),n,lt);
	is->n = n;
	switch(ret)
	{
		case 'v': R k(0,"@",r1(x),is,0);	//values
		case 'i': R is;						//indices
		case 'b':							//boolean mask
		{
			K bs = ktn(KB,xn);
			__builtin_memset(kG(bs),0,xn);
			G* gs = kG(bs); J* js = kJ(is);
			for(J i=0;i<n;++i)
				gs[js[i]] = 1;
			r0(is);
			R bs;
		}
	}
	R krr("how?");
}

Z K selectk(K _n, K x, I lt, C ret)
{
	if(99==xt)
	{
		K r = selectkv(_n,xy,lt,ret);
		if(!r) R 0;	//error already set
		switch(ret)
		{
			case 'v': R r;						//values
			case 'i': R k(0,"@",r1(xx),r,0);	//keys
			case 'b': R xD(r1(xx),r);			//boolean mask dictionary
		}
		R krr("how?");
	}
	else
		R selectkv(_n,x,lt,ret);
}

Z K kth_element(K _kth, K x, I lt)
{
	if(99==xt) x = xy;
	if(!valid_x(x)) R krr("array type");
	if(_kth->t != -KJ) R krr("k type");
	J kth = _kth->j;
	//__builtin_printf("#kth: %lld, xn: %lld, lt: %i\n",kth,xn,lt);
	if(kth<1) return krr("range");
	if(kth>xn)
	{
		switch(xt)
		{
			case KG: case KC: case KH: 
				R ki(lt?-wi:wi);
			case KI: case KM: case KD: case KU: case KV: case KT: 
				R ktj(-xt,lt?-wi:wi);
			case KJ: case KP: case KN:
				R ktj(-xt,lt?-wj:wj);
			case KE: 
				R k(0,"`real$",kf(lt?-wf:wf),0);
			case KF:
				R kf(lt?-wf:wf);
			case KZ:
				R x=kf(lt?-wf:wf),x->t=-KZ,x;
		}
	}
	K is = ktn(KJ,kth>SMALL_K?xn:kth);
	x_select_k(x,kJ(is),kth,lt);
	K res = k(0,"@",r1(x),kj(kJ(is)[kth-1]),0);
	r0(is);
	R res;
}

#define sel(lt,ret) sel##lt##ret
#define seldef(lt,ret) Z K sel(lt,ret)(K x,K y) {R selectk(x,y,lt,*#ret);}
seldef(0,v)seldef(1,v)seldef(0,i)seldef(1,i)seldef(0,b)seldef(1,b)
#define kth(lt) kth##lt
#define kthdef(lt) Z K kth(lt)(K x,K y) {R kth_element(x,y,lt);}
kthdef(0)kthdef(1)

__attribute__((visibility("default")))
K1(kexport)
{
	R k(0,"`kmin`kmax`ikmin`ikmax`bkmin`bkmax`kthmin`kthmax!",knk(8,
		dl((V*)sel(0,v),2),dl((V*)sel(1,v),2),
		dl((V*)sel(0,i),2),dl((V*)sel(1,i),2),
		dl((V*)sel(0,b),2),dl((V*)sel(1,b),2),
		dl((V*)kth(0),2),dl((V*)kth(1),2)
	),0);
}
#ifndef SELECT_H
#define SELECT_H


#include "math.h"
#define KXVER 3
#include "k.h"

#define SMALL_K 400

#define signum(x) ({ __typeof__(x) _x = (x); (_x>0) - (_x<0); })
#define swap(a,b) ({ __typeof__(a) _t = (a); (a) = (b); (b) = _t; })
#define max(a,b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#define min(a,b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a < _b ? _a : _b; })

#define lt(a,b)	((a)>(b))
#define st(a,b)	((a)<(b))

#endif //SELECT_H
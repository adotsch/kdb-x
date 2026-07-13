#include "selectk.h"

#ifndef REL
#define REL lt
#endif

#ifndef T
#define T F
#endif

#ifndef select_k
#define select_k select_k_lt_F
#endif

#ifndef select_k0
#define select_k0 select_k0_lt_F
#endif

/**
 * @brief Floyd–Rivest
 */
static void select_k(T* array, J* index, J left, J right, J k)
{
	while(right>left)
	{
		// Use select recursively to sample a smaller set of size s
        // the arbitrary constants 600 and 0.5 are used in the original
        // version to minimize execution time.
		if((right-left) > 600)
		{
			J n = right - left + 1;
			J i = k - left + 1;
			F z = log(n);
			F s = 0.5 * exp(2 * z/3);
			F sd = 0.5 * sqrt(z * s * (n - s)/n) * signum(i - n/2);
			J newLeft = max(left, (J)(k - i * s/n + sd));
			J newRight = min(right, (J)(k + (n - i) * s/n + sd));
			select_k(array, index, newLeft, newRight, k);
		}
		// partition the elements between left and right around t
		T t = array[index[k]];
		J i = left;
		J j = right;
		swap(index[left],index[k]);
		if(REL(t,array[index[right]]))
			swap(index[right],index[left]);
		while(i < j)
		{
			swap(index[i],index[j]);
			++i;
			--j;
			while(REL(array[index[i]],t))
				++i;
			while(REL(t,array[index[j]]))
				--j;
		}
		if(array[index[left]]==t)
			swap(index[left],index[j]);
		else
		{
			++j;
			swap(index[j],index[right]);
		}
		// Adjust left and right towards the boundaries of the subset
		// containing the (k - left + 1)th smallest element.
		if (j<=k)
			left = j + 1;
		if (k<=j)
			right = j - 1;
	}
}

/**
 * @brief naive select, fast for small k
 */
static void select_k0(T* array, J* index, J n, J k)
{
	if(k==0) R;
	T buff[SMALL_K];
	J im = 0;
	for(J i=0;i<k;i++)
	{
		buff[i] = array[i];
		if(REL(buff[im],buff[i])) im=i;
	}
	for(J i=k;i<n;i++)
		if(REL(array[i],buff[im]))
		{
			index[im] = i;
			buff[im] = array[i];
			for(J j=0;j<k;j++) if(REL(buff[im],buff[j])) im=j;
		}
	swap(index[im],index[k-1]);
	R;
}

#undef REL
#undef T
#undef select_k
#undef select_k0
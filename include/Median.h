#ifndef ZJ_Median__H
#define ZJ_Median__H

#include <stdlib.h>

typedef struct median
{
	double * vector;
	size_t vec_len;
	size_t elem_cur_pos;

	void 
		(*reset)(struct median * mp);

	void 
		(*add)(double value,struct median * mp);

	double 
		(*get_median)(struct median * mp);

	void 
		(*print)(struct median * mp);
}Median, * MedianPtr;

MedianPtr
		Median_constructor();

#endif

#ifndef ZJ_Median__C
#define ZJ_Median__C

#include "../include/Median.h"
#include <malloc.h>
#include <stdlib.h>

void
	median_reset(MedianPtr mp)
	{
		size_t i;
		for(i = 0; i < mp->elem_cur_pos; i++)
		{
			*(mp->vector + i) = 0.0;
		}
		mp->elem_cur_pos = 0;
	}

int 
	median_cmp(const void *a,const void *b)
	{
		return *(double *)a > *(double *)b ? 1 : -1;
	}

void
	median_add(double value,MedianPtr mp)
	{
		if(mp->elem_cur_pos == mp->vec_len)
		{
			mp->vec_len += mp->vec_len / 3;
			mp->vector = (double *)realloc(mp->vector,sizeof(double)*mp->vec_len);
		}
		if(mp->vector != NULL)
		{
			*(mp->vector + mp->elem_cur_pos++) = value;
			qsort(mp->vector,mp->elem_cur_pos,sizeof(mp->vector[0]),median_cmp);
		}
		else
			printf("vector cannot increase!\n");
	}

double
	median_get_median(MedianPtr mp)
	{
		double median;
		if(mp->elem_cur_pos == 0)
		{
			printf("There are no values!\n");
			return 0.0;
		}
		else
		{
			size_t vec_ele_size = mp->elem_cur_pos;
			if(mp->elem_cur_pos % 2 == 0)
			{
				median = (mp->vector[vec_ele_size/2 - 1] + mp->vector[vec_ele_size/2]) / 2;
			}
			else
			{
				median = mp->vector[(vec_ele_size - 1) / 2];
			}
		}
		return median;
	}

void
	median_print(MedianPtr mp)
	{
		size_t i;
		size_t count = 0;
		for(i = 0; i < mp->elem_cur_pos; i++)
		{
			printf("%lf ",mp->vector[i]);
			count++;
			if(count % 10 == 0) printf("\n");
		}
	}

MedianPtr
	Median_constructor()
	{
		MedianPtr mp = (MedianPtr)malloc(sizeof(Median));
		mp->elem_cur_pos = 0;
		mp->vec_len = 3;
		mp->vector = (double *)calloc(mp->vec_len,sizeof(double));
		mp->reset = &median_reset;
		mp->add = &median_add;
		mp->get_median = &median_get_median;
		mp->print = &median_print;
		return mp;
	}

#endif

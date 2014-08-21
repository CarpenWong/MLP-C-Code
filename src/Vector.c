#ifndef ZJ_Vector__C
#define ZJ_Vector__C

#include "../include/Vector.h"
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

size_t
	vector_getsize(struct vec * vp)
	{
		return vp->N;
	}

void
	vector_fill(double value,struct vec * vp)
	{
		size_t i;
		for(i = 0; i < vp->N; i++)
		{
			vp->vector[i] = value;
		}
	}

double
	vector_get(size_t i,struct vec * vp)
	{
		return *(vp->vector + i);
	}

void
	vector_set(size_t i,double value,struct vec * vp)
	{
		if(i >= vp->N)
		{
			printf("Wrong i value!\n");
		}
		else
		{
			*(vp->vector + i) = value;
		}
	}

void
	vector_add(struct vec * v,struct vec * vp)
	{
		size_t i;
		for(i = 0; i < vp->N;i++)
		{
			*(vp->vector + i) += *(v->vector + i);
		}
	}

void
	vector_sub(struct vec * v,struct vec * vp)
	{
		size_t i;
		for(i = 0; i < vp->N;i++)
		{
			*(vp->vector + i) -= *(v->vector + i);
		}
	}

void
	vector_mul(double k,struct vec * vp)
	{
		size_t i;
		for(i = 0; i < vp->N; i++)
		{
			*(vp->vector + i) = *(vp->vector + i) * k;
		}
	}

void
	vector_inc_mul(double k,struct vec * v,struct vec * vp)
	{
		size_t i;
		for(i = 0; i < vp->N; i++)
		{
			*(vp->vector + i) += *(v->vector + i) * k;
		}
	}

double
	vector_dot(struct vec * v,struct vec * vp)
	{
		double sum = 0.0;
		size_t i;
		for(i = 0; i < vp->N; i++)
		{
			sum += vp->vector[i] * v->vector[i];
		}
		return sum;
	}

double
	vector_square_norm(struct vec * vp)
	{
		double sum = 0.0;
		size_t i;
		for(i = 0; i < vp->N; i++)
		{
			sum += pow(*(vp->vector + i),2.0);
		}
		return sum;
	}

double
	vector_norm(struct vec * vp)
	{
		return sqrt(vector_square_norm(vp));
	}

double
	vector_sum(struct vec * vp)
	{
		double sum;
		size_t i;
		for(i = 0; i < vp->N; i++)
		{
			sum += *(vp->vector + i);
		}
		return sum;
	}

double
	vector_min(struct vec * vp)
	{
		double min = vp->vector[0];
		size_t i;
		for(i = 1; i < vp->N; i++)
		{
			if(min > *(vp->vector + i))
			{
				min = *(vp->vector + i);
			}
		}
		return min;
	}

double
	vector_max(struct vec * vp)
	{
		double max = vp->vector[0];
		size_t i;
		for(i = 1; i < vp->N; i++)
		{
			if(max < *(vp->vector + i))
			{
				max = *(vp->vector + i);
			}
		}
		return max;
	}

void
	vector_print(struct vec * vp)
	{
		size_t i;
		size_t count = 0;
		for(i = 0; i < vp->N; i++)
		{
			printf("%lf ",vp->vector[i]);
			count++;
			if(count % 10 == 0) printf("\n");
		}
		printf("\n");
	}

VectorPtr
	Vector_constructor(size_t N)
	{
		VectorPtr vp = (VectorPtr)malloc(sizeof(Vector));
		vp->N = N;
		vp->vector = (double *)calloc(N,sizeof(double));

		vp->getsize = &vector_getsize;
		vp->fill = &vector_fill;
		vp->get = &vector_get;
		vp->set = &vector_set;
		vp->add = &vector_add;
		vp->sub = &vector_sub;
		vp->mul = &vector_mul;
		vp->inc_mul = &vector_inc_mul;
		vp->dot = &vector_dot;
		vp->square_norm = &vector_square_norm;
		vp->norm = &vector_norm;
		vp->sum = &vector_sum;
		vp->min = &vector_min;
		vp->max = &vector_max;
		vp->print = &vector_print;

		return vp;
	}
#endif

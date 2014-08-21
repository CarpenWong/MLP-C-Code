#ifndef ZJ_Vector__H
#define ZJ_Vector__H

#include <stdlib.h>

typedef struct vec
{
	size_t N;
	double * vector;

	size_t
		(*getsize)(struct vec * vp);

	void
		(*fill)(double value,struct vec * vp);

	double
		(*get)(size_t i,struct vec * vp);

	void
		(*set)(size_t i,double value,struct vec * vp);

	void
		(*add)(struct vec * v,struct vec * vp);

	void
		(*sub)(struct vec * v,struct vec * vp);

	void
		(*mul)(double k,struct vec * vp);

	void
		(*inc_mul)(double k,struct vec * v,struct vec * vp);

	double
		(*dot)(struct vec * v,struct vec * vp);

	double
		(*square_norm)(struct vec * vp);

	double
		(*norm)(struct vec * vp);

	double
		(*sum)(struct vec * vp);

	double
		(*min)(struct vec * vp);

	double
		(*max)(struct vec * vp);

	void
		(*print)(struct vec * vp);
}Vector , * VectorPtr;

VectorPtr
		Vector_constructor(size_t N);

#endif

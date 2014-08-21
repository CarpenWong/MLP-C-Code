#ifndef ZJ_MLP__H
#define ZJ_MLP__H

#include "Vector.h"
#include "VectorList.h"
#include "VectorListOps.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>

#define PI 3.14159265

typedef enum {positive = 1,negative = -1} PNType;

typedef struct mlp
{
	size_t N;
	size_t K;
	VectorPtr * V;
	VectorPtr vn;
	VectorPtr W;
	double wn;
	VectorPtr Y;
	VectorPtr Yp;
	VectorPtr Roin;
	double rout;
	double z;
	double zp;
	double eta;
	bool inital_V;/*if true ,inital V;if false ,inital W*/
	VectorPtr max_vec;
	VectorPtr min_vec;

	bool
		(* classify)(VectorPtr X,struct mlp *);

	void
		(* update)(VectorPtr X,bool inClass,struct mlp *);

	void
		(*reset)(struct mlp *);
}MLP, * MLPtr;

MLPtr
	MLP_Constructor(size_t k,size_t n);

#endif

#ifndef ZJ_Vectorlistops__H
#define ZJ_Vectorlistops__H

#include <stdlib.h>
#include <stdio.h>
#include "Vector.h"
#include "Median.h"
#include "KahanSum.h"
#include "VectorList.h"


typedef struct vec_list_ops
{
	size_t cols,rows;

	size_t
		(*get_cols)(struct vec_list_ops * vlop);

	size_t
		(*get_rows)(struct vec_list_ops * vlop);

	void
		(*print)(VectorListPtr vlp);

	void
		(*write_data)(FILE * fp,char * path,VectorListPtr vlp,size_t * point_ids);

	VectorListPtr
		(*read_data)(FILE * fp,char * path,struct vec_list_ops * vlop,size_t **point_ids);

	VectorPtr
		(*vector_mean)(VectorListPtr vlp,struct vec_list_ops * vlop);

	VectorPtr
		(*vector_median)(VectorListPtr vlp,struct vec_list_ops * vlop);

}VectorLisOp , * VectorLisOpPtr;

VectorLisOpPtr
	VectorLisOpPtr_constructor();

#endif

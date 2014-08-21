#ifndef ZJ_VectorList__H
#define ZJ_VectorList__H


typedef struct vec_list
{
	VectorPtr vp;
	struct vec_list * next;
}VectorList, * VectorListPtr;

#endif

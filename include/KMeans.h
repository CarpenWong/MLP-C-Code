#ifndef ZJ_KMeans__H
#define ZJ_KMeans__H

#include "VectorListOps.h"
#include <float.h>
#include <stdbool.h>
#include <string.h>

typedef struct kmns
{
	VectorPtr * cluster_centers;
	size_t * point_ids;
	size_t num_cluster;
	size_t num_points;

	VectorPtr
		(*get_cluster_center)(size_t i,struct kmns * km);

	size_t
		(*get_point_id)(size_t i,struct kmns * km);

	size_t
		(*get_closest_center_id)(VectorPtr x,struct kmns * km);

	void
		(*clear_cluster_centers)(struct kmns * km);

	void
		(*compute_cluster_centers)(VectorListPtr tvlp,struct kmns * km);

	void
		(*set_num_cluster)(size_t num,struct kmns * km);

	void
		(*set_num_points)(size_t num,struct kmns * km);

	void
		(*init_point_indexes)(struct kmns * km);

	void
		(*init_cluster_centers)(struct kmns * km);

	bool
		(*check_convergence)(size_t * init_pids,size_t * current_pids,struct kmns * km);

	size_t
		(*clusterize)(VectorListPtr tvlp,struct kmns * km);

	void
		(*init_point_plusplus)(size_t inDim,VectorListPtr tvlp,struct kmns * km);

	double
		(*DunnIndex)(VectorListPtr tvlp,struct kmns * km);

	double
		(*DaviesBouldinIndex)(VectorListPtr tvlp,struct kmns * km);

}KMeans, * KMeansPtr;

KMeansPtr
	KMeans_constructor();

#endif

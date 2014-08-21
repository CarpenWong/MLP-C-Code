#ifndef ZJ_KMeans__C
#define ZJ_KMeans__C

#include "../include/KMeans.h"

VectorPtr
	kmeans_get_cluster_center(size_t i,struct kmns * km)
	{
		return km->cluster_centers[i];
	}

size_t
	kmeans_get_point_id(size_t i,struct kmns * km)
	{
		return km->point_ids[i];
	}

size_t
	kmeans_get_closest_center_id(VectorPtr x,struct kmns * km)
	{
		size_t id = 0;
		double dis = DBL_MAX;
		double tmp_dis = 0.0;
		size_t i;

		for(i = 0; i < km->num_cluster; i++)
		{
			km->cluster_centers[i]->sub(x,*(km->cluster_centers + i));
			tmp_dis = km->cluster_centers[i]->norm(*(km->cluster_centers + i));
			if(tmp_dis < dis)
			{
				dis = tmp_dis;
				id = i;
			}
			km->cluster_centers[i]->add(x,*(km->cluster_centers + i));
		}
		return id;
	}

void
	kmeans_clear_cluster_centers(struct kmns * km)
	{
		size_t i;
		for(i = 0; i < km->num_cluster; i++)
		{
			if(km->cluster_centers[i] != NULL)
			{
				free(km->cluster_centers[i]->vector);
				free(km->cluster_centers[i]);
				km->cluster_centers[i] = NULL;
			}
		}
	}

void
	kmeans_compute_cluster_centers(VectorListPtr tvlp,struct kmns * km)
	{
		kmeans_clear_cluster_centers(km);
		size_t i;
		VectorListPtr * vlp = (VectorListPtr *)calloc(km->num_cluster,sizeof(VectorListPtr));

		for(i = 0; i < km->num_cluster; i++)
		{
			*(vlp + i) = (VectorListPtr)malloc(sizeof(VectorList));
			vlp[i]->vp = NULL;
			vlp[i]->next = NULL;
		}

		i = 0;
		VectorListPtr p;
		p = tvlp->next;
		size_t N = p->vp->N;
		size_t id;
		VectorListPtr vlp_p;

		for(i = 0; i < km->num_points; i++)
		{
			id = km->point_ids[i];
			vlp_p = vlp[id];
			while(vlp_p->next != NULL)
			{
				vlp_p = vlp_p->next;
			}
			VectorPtr tmp_vp = Vector_constructor(N);
			memcpy(tmp_vp->vector,p->vp->vector,N*sizeof(double));
			VectorListPtr tmp_vlp = (VectorListPtr)malloc(sizeof(VectorList));
			tmp_vlp->vp = tmp_vp;
			tmp_vlp->next = NULL;
			vlp_p->next = tmp_vlp;
			p = p->next;
		}

		VectorLisOpPtr vlop = VectorLisOpPtr_constructor();
		vlop->cols = N;
		for(i = 0; i < km->num_cluster; i++)
		{
			km->cluster_centers[i] = vlop->vector_median(vlp[i],vlop);
		}

		/*free resource*/
		free(vlop);
		VectorListPtr f_vlp;
		for(i = 0;i < km->num_cluster; i++)
		{
			f_vlp = vlp[i];
			p = f_vlp->next;
			while(p != NULL)
			{
				f_vlp->next = p->next;
				free(p->vp->vector);
				free(p->vp);
				free(p);
				p = f_vlp->next;
			}
		}
		free(vlp);
	}

void
	kmeans_set_num_cluster(size_t num,struct kmns * km)
	{
		km->num_cluster = num;
	}

void
	kmeans_set_num_points(size_t num,struct kmns * km)
	{
		km->num_points = num;
	}

void
	kmeans_init_point_indexes(struct kmns * km)
	{
		km->point_ids = (size_t *)calloc(km->num_points,sizeof(size_t));
		size_t i;
		for(i = 0; i < km->num_points; i++)
		{
			km->point_ids[i] = ((size_t)(rand()) % km->num_cluster);
		}
	}

void
	kmeans_init_cluster_centers(struct kmns * km)
	{
		km->cluster_centers = (VectorPtr *)calloc(km->num_cluster,sizeof(VectorPtr));
		size_t i;
		for(i = 0; i < km->num_cluster; i++)
		{
			km->cluster_centers[i] = NULL;
		}
	}

bool
	kmeans_check_convergence(size_t * init_pids,size_t * current_pids,KMeansPtr km)
	{
		size_t i;
		for(i = 0; i < km->num_points; i++)
		{
			if(init_pids[i] != current_pids[i])
				return false;
		}
		return true;
	}

size_t
	kmeans_clusterize(VectorListPtr tvlp,struct kmns * km)
	{
		size_t * current_pids = (size_t *)calloc(km->num_points,sizeof(size_t));
		size_t iterate_count = 0;
		bool iterate_flag = true;
		size_t i;
		VectorListPtr p;

		while(iterate_flag)
	 	{
			kmeans_compute_cluster_centers(tvlp,km);
			p = tvlp->next;
			for(i = 0; i < km->num_points; i++)
			{
				current_pids[i] = kmeans_get_closest_center_id(p->vp,km);
				p = p->next;
			}

			if(kmeans_check_convergence(km->point_ids,current_pids,km))
			{
				iterate_flag = false;
				iterate_count++;
				printf("Iterate #%d.\n",iterate_count);
			}
			else
			{
				memcpy(km->point_ids,current_pids,sizeof(size_t)*km->num_points);
				iterate_count++;
				printf("Iterate #%d.\n",iterate_count);
			}
		}
		free(current_pids);
		return iterate_count;
	}

void
	kmeans_init_point_plusplus(size_t inDim,VectorListPtr tvlp,struct kmns * km)
	{
		size_t pos = 0,subpoint = 0;
		enum{EMPTY = 0,TAKEN = 1024};
		double *dist = (double *)calloc(inDim,sizeof(double));

		if(inDim == 0) return;

		if(km->point_ids != NULL) free(km->point_ids);

		km->point_ids = (size_t *)calloc(inDim,sizeof(size_t));

		for(pos = 0; pos < inDim; pos++)
		{
			km->point_ids[pos] = EMPTY;
		}

		subpoint = (size_t)rand() % inDim;

		pos = 0;
		VectorListPtr p = tvlp->next;
		size_t N = p->vp->N;
		while(p != NULL)
		{
			if(pos == subpoint)
			{
				VectorPtr tmp_vp = Vector_constructor(N);
				memcpy(tmp_vp->vector,p->vp->vector,N*sizeof(double));
				km->cluster_centers[0] = tmp_vp;
				km->point_ids[pos] = TAKEN;
			}
			pos++;
			p = p->next;
		}

		pos = 0;
		p = tvlp->next;
		while(p != NULL)
		{
			if(pos != subpoint)
			{
				km->cluster_centers[0]->sub(p->vp,*(km->cluster_centers + 0));
				dist[pos] = km->cluster_centers[0]->square_norm(*(km->cluster_centers + 0));
				km->cluster_centers[0]->add(p->vp,*(km->cluster_centers + 0));
			}
			pos++;
			p = p->next;
		}

		double distSum = 0.0;
		double sum = 0.0;
		int nextPointsub = -1;

		size_t i;
		for(i = 0; i < km->num_cluster; i++)
		{
			for(pos = 0; pos < inDim; pos++)
			{
				if(km->point_ids[pos] != TAKEN)
					distSum += dist[pos];
			}

			double r = 0.0;
			size_t k;
			for(k = 0;k < 10;k++)
			{
				r = 1 + (int)(10.0*rand()/(RAND_MAX + 1.0));
			}

			for(pos = rand()%inDim; pos != inDim; pos++)
			{
				if(km->point_ids[pos] != TAKEN)
				{
					sum += dist[pos];
					if(sum > r)
					{
						nextPointsub = pos;
						break;
					}
				}
			}

			if(nextPointsub == -1)
			{
				for(pos = inDim -1; pos > 0; pos--)
				{
					if(km->point_ids[pos] != TAKEN)
					{
						nextPointsub = pos;
						break;
					}
				}
			}

			if(nextPointsub >= 0)
			{
				pos = 0;
				p = tvlp->next;
				while(p != NULL)
				{
					if(pos == (size_t)nextPointsub)
					{
						VectorPtr tmp_vp = Vector_constructor(N);
						memcpy(tmp_vp->vector,p->vp->vector,N*sizeof(double));
						km->cluster_centers[k] = tmp_vp;
						km->point_ids[pos] = TAKEN;
					}
					pos++;
					p = p->next;
				}

				if( k < km->num_cluster)
				{
					p = tvlp->next;
					for(pos = 0; pos < inDim;p = p->next,pos++)
					{
						if(km->point_ids[pos] != TAKEN)
						{
							km->cluster_centers[0]->sub(p->vp,*(km->cluster_centers + 0));
							double tmpd = km->cluster_centers[0]->square_norm(*(km->cluster_centers + 0));
							if(tmpd < dist[pos])
							{
								dist[pos] = tmpd;
							}
							km->cluster_centers[0]->add(p->vp,*(km->cluster_centers + 0));
						}
					}
				}
			}
			else
			{
				break;
			}
		}
		free(dist);
		p = tvlp->next;
		for(pos = 0; pos < km->num_points; pos++)
		{
			km->point_ids[pos] = kmeans_get_closest_center_id(p->vp,km);
			p = p->next;
		}
	}

double
	kmeans_DunnIndex(VectorListPtr tvlp,struct kmns * km)
	{
		double mindc = 0.0;
		double maxdist = 0.0;
		double tmpd = 0.0;
		size_t pos = 0;
		VectorListPtr p;

		km->cluster_centers[0]->sub(*(km->cluster_centers + 1),*(km->cluster_centers + 0));
		mindc = km->cluster_centers[0]->square_norm(*(km->cluster_centers + 0));
		km->cluster_centers[0]->add(*(km->cluster_centers + 1),*(km->cluster_centers + 0));

		size_t posi;
		for(pos = 0; pos < km->num_cluster;pos++)
		{
			for(posi = pos + 1; posi < km->num_cluster; posi++)
			{
				km->cluster_centers[pos]->sub(*(km->cluster_centers + posi),*(km->cluster_centers + pos));
				tmpd = km->cluster_centers[pos]->square_norm(*(km->cluster_centers + pos));
				if(tmpd < mindc)
				{
					mindc = tmpd;
				}
				km->cluster_centers[pos]->add(*(km->cluster_centers + pos),*(km->cluster_centers + posi));
			}
		}

		p = tvlp->next;
		km->cluster_centers[km->point_ids[pos]]->sub(p->vp,*(km->cluster_centers + km->point_ids[pos]));
		maxdist = km->cluster_centers[km->point_ids[pos]]->norm(*(km->cluster_centers + km->point_ids[pos]));
		pos = 1;
		while(p != NULL)
		{
			km->cluster_centers[km->point_ids[pos]]->sub(p->vp,*(km->cluster_centers + km->point_ids[pos]));
			tmpd = km->cluster_centers[km->point_ids[pos]]->norm(*(km->cluster_centers + km->point_ids[pos]));
			if((tmpd - maxdist) > 0.0)
			{
				maxdist = tmpd;
			}
			km->cluster_centers[km->point_ids[pos]]->add(p->vp,*(km->cluster_centers + km->point_ids[pos]));
			pos++;
			p = p->next;
		}
		return (mindc / maxdist);
	}

double
	kmeans_DaviesBouldinIndex(VectorListPtr tvlp,struct kmns * km)
	{
		double **avgdctr = (double **)calloc(km->num_cluster,sizeof(double *));
		size_t dim = km->num_points;
		double maxdist = 0.0,tmpd = 0.0;
		size_t pos = 0,posi = 0;
		int numinclusters = 0;
		double sumdist = 0.0;
		double *avgdistoctr = (double *)calloc(km->num_cluster,sizeof(double));

		for(pos = 0; pos < km->num_cluster; pos++)
		{
			avgdctr[pos] = (double *)calloc(km->num_cluster,sizeof(double));
			for(posi = 0; posi < km->num_cluster; posi++)
			{
				avgdctr[pos][posi] = 0.0;
			}
		}

		VectorListPtr p;
		for(pos = 0; pos < km->num_cluster; pos++)
		{
			p = tvlp->next;
			for(posi = 0; posi < dim; p = p->next,posi++)
			{
				if(km->point_ids[posi] == pos)
				{
					++numinclusters;
					km->cluster_centers[pos]->sub(p->vp,*(km->cluster_centers + pos));
					sumdist += km->cluster_centers[pos]->norm(*(km->cluster_centers + pos));
					km->cluster_centers[pos]->add(p->vp,*(km->cluster_centers + pos));
				}
			}
			avgdistoctr[pos] = sumdist / numinclusters;
		}

		for(pos = 0; pos < km->num_cluster; pos++)
		{
			for(posi = pos + 1; posi < km->num_cluster; posi++)
			{
				km->cluster_centers[pos]->sub(km->cluster_centers[posi],*(km->cluster_centers + pos));
				avgdctr[pos][posi] = km->cluster_centers[pos]->norm(*(km->cluster_centers + pos));
				avgdctr[posi][pos] = avgdctr[pos][posi];
				km->cluster_centers[pos]->add(km->cluster_centers[posi],*(km->cluster_centers + pos));
			}
		}

		sumdist = (avgdistoctr[0] + avgdistoctr[1]) / avgdctr[0][1];
		for(pos = 0; pos < km->num_cluster;pos++)
		{
			for(posi = 0; posi < km->num_cluster; posi++)
			{
				if(pos != posi)
				{
					tmpd = (avgdistoctr[pos] + avgdistoctr[posi])/avgdctr[pos][posi];
					if((maxdist - tmpd) < 0.0)
					{
						maxdist = tmpd;
					}
				}
			}
			sumdist += maxdist;
		}

		for(pos = 0; pos < km->num_cluster; pos++)
			free(avgdctr[pos]);
		free(avgdctr);
		free(avgdistoctr);

		return sumdist / km->num_cluster;
	}

KMeansPtr
	KMeans_constructor()
	{
		KMeansPtr km = (KMeansPtr)malloc(sizeof(KMeans));
		km->num_points = 0;
		km->num_cluster = 0;

		km->get_cluster_center = &kmeans_get_cluster_center;

		km->get_point_id = &kmeans_get_point_id;

		km->get_closest_center_id = &kmeans_get_closest_center_id;

		km->clear_cluster_centers = &kmeans_clear_cluster_centers;

		km->compute_cluster_centers = &kmeans_compute_cluster_centers;

		km->set_num_cluster = &kmeans_set_num_cluster;

		km->set_num_points = &kmeans_set_num_points;

		km->init_point_indexes = &kmeans_init_point_indexes;

		km->init_cluster_centers = &kmeans_init_cluster_centers;

		km->check_convergence = &kmeans_check_convergence;

		km->clusterize = &kmeans_clusterize;

		km->init_point_plusplus = &kmeans_init_point_plusplus;

		km->DunnIndex = &kmeans_DunnIndex;

		km->DaviesBouldinIndex = &kmeans_DaviesBouldinIndex;

		return km;
	}
#endif

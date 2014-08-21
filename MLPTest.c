#include "./include/MLP.h"
#include <stdio.h>
#include <malloc.h>

size_t 
	calc_hit_count(size_t rows,size_t * point_ids,size_t * tmp_point_ids,MLPtr mlp,VectorListPtr vlp)
{
	size_t i = 0;
	size_t hit_count = 0;
	bool flag;
	VectorListPtr tmp_vlp = vlp->next;
	while(tmp_vlp != NULL)
	{
		flag = mlp->classify(tmp_vlp->vp,mlp);
		if(flag)
		{
			tmp_point_ids[i] = 1;
		}
		else
		{
			tmp_point_ids[i] = 0;
		}
		tmp_vlp = tmp_vlp->next;
		i++;
	}
	for(i = 0;i < rows;i++)
	{
		if(tmp_point_ids[i] == point_ids[i]) hit_count++;
	}
	if(hit_count < rows /2)
	{
		hit_count = rows - hit_count;
	}
	return hit_count;
}

void
Min_Max_Initial(VectorListPtr vlp,MLPtr mlp,size_t cols)
{
	double min = DBL_MAX;
	double max = -DBL_MAX;
	double tmp_max_min;
	size_t i;
	VectorListPtr tmp_vlp;
	mlp->min_vec = Vector_constructor(cols);
	mlp->max_vec = Vector_constructor(cols);
	for(i = 0; i < cols; i++)
	{
		tmp_vlp = vlp->next;
		while(tmp_vlp != NULL)
		{
			tmp_max_min = tmp_vlp->vp->vector[i];
			if(tmp_max_min > max)
			{
				max = tmp_max_min;
			}
			if(tmp_max_min < min)
			{
				min = tmp_max_min;
			}
			tmp_vlp = tmp_vlp->next;
		}
		mlp->min_vec->vector[i] = min;
		mlp->max_vec->vector[i] = max;
		min = DBL_MAX;
		max = -DBL_MAX;
	}
}

int
main(int argc,char **argv)
{
	if(argc != 4)
	{
		printf("There is not enough paramenter!\n");
		return -1;
	}
	else
	{
		FILE * p = NULL;
		char * path_in = *(argv + 1);
		char * path_out = *(argv + 2);
		size_t k = (size_t)atoi(*(argv + 3));
		size_t *point_ids;
		size_t *check_point_ids;
		size_t rows,cols;
		size_t i;
		MLPtr mlp;

		VectorLisOpPtr vlop = VectorLisOpPtr_constructor();
		VectorListPtr vlp = vlop->read_data(p,path_in,vlop,&point_ids);
		if(vlp != NULL)
		{
			VectorListPtr tmp_vlp;

			/*get rows and cols of dataset*/
			cols = vlop->get_cols(vlop);
			rows = vlop->get_rows(vlop);

			mlp = MLP_Constructor(k,cols);

			mlp->eta = 0.05;

			/*initial*/
			Min_Max_Initial(vlp,mlp,cols);
			mlp->reset(mlp);

			/*iterate : end of iterate's steps*/
			check_point_ids = (size_t *)calloc(rows,sizeof(size_t));
			size_t *tmp_point_ids = (size_t *)calloc(rows,sizeof(size_t));
			size_t hit_count = 0,tmp_hit_count;
			size_t j;
			i = 0;
			while(i < 5000)
			{
				tmp_vlp = vlp->next;
				j = 0;
				while(tmp_vlp != NULL)
				{
					mlp->classify(tmp_vlp->vp,mlp);
					mlp->update(tmp_vlp->vp,point_ids[j],mlp);
					tmp_vlp = tmp_vlp->next;
					j++;
				}
				tmp_hit_count = calc_hit_count(rows,point_ids,tmp_point_ids,mlp,vlp);
				if(tmp_hit_count > hit_count)
				{
					hit_count = tmp_hit_count;
					tmp_hit_count = 0;
					memcpy(check_point_ids,tmp_point_ids,sizeof(size_t)*rows);
				}
				i++;
			}

			vlop->write_data(p,path_out,vlp,check_point_ids);
			printf("Hit rate:%f\n",(double)hit_count / (double)rows);

		free(tmp_point_ids);
		free(point_ids);
		}
		return 0;
	}
}

#ifndef ZJ_Vectorlistops__C
#define ZJ_Vectorlistops__C

#include "../include/VectorListOps.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

size_t
	Vector_list_ops_get_cols(VectorLisOpPtr vlop)
	{
		return vlop->cols;
	}

size_t
	Vector_list_ops_get_rows(VectorLisOpPtr vlop)
	{
		return vlop->rows;
	}

void
	Vector_list_ops_print(VectorListPtr vlp)
	{
		VectorListPtr p;
		p = vlp->next;
		while(p != NULL)
		{
			p->vp->print(p->vp);
			p = p->next;
		}
	}

void
	Vector_list_ops_write_data(FILE * fp,char * path,VectorListPtr vlp,size_t * point_ids)
	{
		if((fp = fopen(path,"w")) != NULL)
		{
			VectorListPtr p = vlp->next;
			size_t cols = p->vp->N;
			size_t i,j;
			j = 0;
			while(p != NULL)
			{
				for(i = 0; i < cols; i++)
				{
					fprintf(fp,"%.10f%c",p->vp->vector[i],' ');
				}
				fprintf(fp,"%d%c",point_ids[j],'\n');
				p = p->next;
				j++;
			}
			fclose(fp);
		}
		else
		{
			printf("Cannot Open this file : %s\n",path);
		}
	}

VectorListPtr
	Vector_list_ops_read_data(FILE * fp,char * path,VectorLisOpPtr vlop,size_t **point_ids)
	{
		if((fp = fopen(path,"r")) != NULL)
		{
			VectorListPtr head = (VectorListPtr)malloc(sizeof(VectorList));
			head->vp = NULL;
			head->next = NULL;
			char ch;
			bool flag = false;
			size_t word_count = 0;
			do
			{
				ch = fgetc(fp);
				if(isdigit(ch) || ch == 'e' || ch == '+' || ch == '-' || ch == '.')
				{
					flag = true;
				}else if((ch == ' '||ch == '\n')&& flag == true)
				{
					word_count++;
					flag = false;
				}
			}while(ch != '\n');
			vlop->cols = --word_count;
			/*seek to the start of the file*/
			fseek(fp, 0, SEEK_SET);
			double dnum;
			size_t i, line_count = 0;
			VectorListPtr p = head;
			*point_ids = (size_t *)malloc(sizeof(size_t));
			size_t k = 0;
			while(!feof(fp))
			{
				VectorListPtr q = (VectorListPtr)malloc(sizeof(VectorList));
				q->vp = Vector_constructor(word_count);
				q->next = NULL;
				for(i = 0; i < word_count; i++)
				{
					fscanf(fp,"%le ",&dnum);
					q->vp->set(i,dnum,q->vp);
				}
				fscanf(fp,"%d\n",*point_ids+k);
				p->next = q;
				p = q;
				line_count++;
				k++;
				*point_ids = (size_t *)realloc(*point_ids,sizeof(size_t)*(k + 1));
			}
			vlop->rows = line_count;
			fclose(fp);
			return head;
		}
		else
		{
			printf("Cannot Open this file : %s\n",path);
			return NULL;
		}
	}

VectorPtr
	Vector_list_ops_vector_mean(VectorListPtr vlp,VectorLisOpPtr vlop)
	{
		size_t i;
		size_t cols = vlop->get_cols(vlop);
		size_t rows = vlop->get_rows(vlop);
		KahanSumPtr mean_sum = KahanSum_constructor();
		VectorPtr mean_vec = Vector_constructor(cols);

		VectorListPtr p;
		for(i = 0; i < cols; i++)
		{
			p = vlp->next;
			while(p != NULL)
			{
				mean_sum->add(p->vp->get(i,p->vp),mean_sum);
				p = p->next;
			}
			mean_vec->set(i,mean_sum->get_sum(mean_sum)/(double)rows,mean_vec);
			mean_sum->reset(mean_sum);
		}
		return mean_vec;
	}

VectorPtr
	Vector_list_ops_vector_median(VectorListPtr vlp,VectorLisOpPtr vlop)
	{
		size_t cols = vlop->get_cols(vlop);
		size_t i;
		MedianPtr median = Median_constructor();
		VectorPtr median_vec = Vector_constructor(cols);

		VectorListPtr p;
		for(i = 0; i < cols; i++)
		{
			p = vlp->next;
			while(p != NULL)
			{
				median->add(p->vp->get(i,p->vp),median);
				p = p->next;
			}
			median_vec->set(i,median->get_median(median),median_vec);
			median->reset(median);
		}
		return median_vec;
	}

VectorLisOpPtr
	VectorLisOpPtr_constructor()
	{
		VectorLisOpPtr vlop = (VectorLisOpPtr)malloc(sizeof(VectorLisOp));
		vlop->cols = 0;
		vlop->rows = 0;

		vlop->get_cols = &Vector_list_ops_get_cols;
		vlop->get_rows = &Vector_list_ops_get_rows;
		vlop->write_data = &Vector_list_ops_write_data;
		vlop->read_data = &Vector_list_ops_read_data;
		vlop->vector_mean = &Vector_list_ops_vector_mean;
		vlop->vector_median = &Vector_list_ops_vector_median;
		vlop->print = &Vector_list_ops_print;
		return vlop;
	}
#endif

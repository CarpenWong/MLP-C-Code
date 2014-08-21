#ifndef ZJ_MLP__C
#define ZJ_MLP__C

#include "../include/MLP.h"

static double
		mlp_generate_random()
		{
			/*srand((size_t)time(0));*/
			srand(47);
			return (1 + (10.0*rand()/(RAND_MAX + 1.0))) / 10.0;
		}

static double
		mlp_box_muller(size_t i)
		{
			double u = mlp_generate_random();
			double v = mlp_generate_random();
			if(i % 2 == 0)
			{
				return sqrt(-2.0 * log(u)) * sin(2.0 * PI * v);
			}
			else
			{
				return sqrt(-2.0 * log(u)) * cos(2.0 * PI * v);
			}
		}

static void
		mlp_generate_N_unit_vecter(VectorPtr * V,MLPtr mlp)
		{
			/*Inital V,it's N coefficients*/
			size_t vdim = mlp->N;
			size_t i;
			/*Inital W,it's K coefficients*/
			if(!mlp->inital_V) vdim = mlp->K;

			*V = Vector_constructor(vdim);
			for(i = 0; i < vdim; i++)
			{
				*((*V)->vector + i) = mlp_box_muller(i);
			}
			double norm = (*V)->norm(*V);
			for(i = 0; i < vdim; i++)
			{
				*((*V)->vector + i) /= norm;
			}
		}

static VectorPtr
		mlp_translate_standard(VectorPtr X,MLPtr mlp)
		{
			size_t N = X->N;
			size_t i;
			VectorPtr tmp_X = Vector_constructor(N);
			memcpy(tmp_X->vector,X->vector,sizeof(double)*N);
			double tmp_interval;
			for(i = 0; i < N; i++)
			{
				tmp_interval = mlp->max_vec->vector[i] - mlp->min_vec->vector[i];
				*(tmp_X->vector + i) = ((tmp_X->vector[i] - mlp->min_vec->vector[i])/tmp_interval)*2.0 + (-1.0);
			}
			/*test*
			printf("X :");
			X->print(X);
			printf("Standard :");
			tmp_X->print(tmp_X);
			*test*/
			return tmp_X;
		}

static void
		mlp_set_hyperplane_weights(size_t i,VectorPtr C,VectorPtr * N,MLPtr mlp)
		{
			if(C == NULL && mlp->inital_V) 
			{
				*(mlp->vn->vector + i) = 0.0;
			}
			else
			{
				mlp->wn = 0.0;
			}
			mlp_generate_N_unit_vecter(N,mlp);
		}

static double
		mlp_signed_dist_y(size_t i,VectorPtr X,MLPtr mlp)
		{
			return X->dot(*(mlp->V + i),X) + 1.0*(*(mlp->vn->vector + i));
		}

static double
		mlp_signed_dist_z(VectorPtr Y,MLPtr mlp)
		{
			return Y->dot(mlp->W,Y) + 1.0*mlp->wn;
		}

static void
		mlp_forward(VectorPtr X,MLPtr mlp)
		{
			size_t i;
			size_t k = mlp->K;
			double tmp_result;
			/*standard point X*/
			VectorPtr tmp_X = mlp_translate_standard(X,mlp);
			for(i = 0; i < k; i++)
			{
				tmp_result = mlp_signed_dist_y(i,tmp_X,mlp);
				mlp->Y->vector[i] = tanh(tmp_result);
				mlp->Yp->vector[i] = 1.0 - pow(tanh(tmp_result),2.0);
			}
			tmp_result = mlp_signed_dist_z(mlp->Y,mlp);
			mlp->z = tanh(tmp_result);
			mlp->zp = 1.0 - pow(tanh(tmp_result),2.0);

			/*free*/
			free(tmp_X->vector);
			free(tmp_X);
		}

static void
		mlp_backward(PNType z_star,MLPtr mlp)
		{
			size_t i;
			size_t k = mlp->K;
			/*mlp->rout = (mlp->z - z_star) * mlp->zp;*/
			mlp->rout = (z_star - mlp->z) * mlp->zp;
			for(i = 0; i < k; i++)
			{
				mlp->Roin->vector[i] = mlp->W->vector[i] * mlp->rout * mlp->Yp->vector[i];
			}
		}

static bool
		mlp_classify(VectorPtr X,MLPtr mlp)
		{
			mlp_forward(X,mlp);
			if(mlp->z < 0) return false;
			else return true;
		}

static void
		mlp_update(VectorPtr X,bool inClass,MLPtr mlp)
		{
			PNType z_star;
			if(inClass) z_star = positive;
			else z_star = negative;
			mlp_backward(z_star,mlp);
			size_t k = mlp->K;
			size_t n = mlp->N;
			size_t i,j;

			/*standard point X*/
			VectorPtr tmp_X = mlp_translate_standard(X,mlp);
			for(i = 0; i < k; i++)
			{
				for(j = 0; j < n; j++)
				{
					mlp->V[i]->vector[j] += mlp->eta * mlp->Roin->vector[i] * tmp_X->vector[j];
				}
				mlp->vn->vector[i] += mlp->eta * mlp->Roin->vector[i] * 1.0;
			}

			for(i = 0; i < k; i++)
			{
				mlp->W->vector[i] += mlp->eta * mlp->Y->vector[i] * mlp->rout;
			}
			mlp->wn += mlp->eta * mlp->rout * 1.0;
			/*free*/
			free(tmp_X->vector);
			free(tmp_X);
		}

static void
		mlp_reset(MLPtr mlp)
		{
			size_t i;
			size_t k = mlp->K;
			/*Inital V and vn*/
			mlp->inital_V = true;
			for(i = 0; i < k; i++)
			{
				mlp_set_hyperplane_weights(i,NULL,mlp->V + i,mlp);
			}
			/*Inital W and wn*/
			mlp->inital_V = false;
			mlp_set_hyperplane_weights(0,NULL,&mlp->W,mlp);
		}

MLPtr
		MLP_Constructor(size_t k,size_t n)
		{
			MLPtr mlp = (MLPtr)malloc(sizeof(MLP));

			mlp->K = k;
			mlp->N = n;

			mlp->Yp = Vector_constructor(k);
			mlp->Y = Vector_constructor(k);
			mlp->V = (VectorPtr *)calloc(k,sizeof(VectorPtr));
			mlp->vn = Vector_constructor(k);
			mlp->Roin = Vector_constructor(k);

			mlp->reset = &mlp_reset;
			mlp->classify = &mlp_classify;
			mlp->update = &mlp_update;
			return mlp;
		}

#endif

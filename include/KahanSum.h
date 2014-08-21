#ifndef ZJ_Kahan__H
#define ZJ_Kahan__H

typedef struct KahanS
{
	double sum;
	double c;

	void
		(* reset)(struct KahanS *ks_ptr);

	void
		(* add)(double value,struct KahanS * ks_ptr);

	double
		(* get_sum)(struct KahanS *ks_ptr);
}KahanSum , * KahanSumPtr;

KahanSumPtr
		KahanSum_constructor();

#endif

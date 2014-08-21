#ifndef ZJ_Kahan__C
#define ZJ_Kahan__C

#include "../include/KahanSum.h"
#include <malloc.h>

void
	kahan_reset(KahanSumPtr ks_ptr)
	{
		ks_ptr->c = 0.0;
		ks_ptr->sum = 0.0;
	}

void
	kahan_add(double value,KahanSumPtr ks_ptr)
	{
		double y,t;
		y = value - ks_ptr->c;
		t = ks_ptr->sum + y;
		ks_ptr->c = (t - ks_ptr->sum) - y;
		ks_ptr->sum = t;
	}

double
	kahan_get_sum(KahanSumPtr ks_ptr)
	{
		return ks_ptr->sum;
	}

KahanSumPtr
	KahanSum_constructor()
	{ 
		KahanSumPtr ks_ptr = (KahanSumPtr)malloc(sizeof(KahanSum));
		ks_ptr->c = 0.0;
		ks_ptr->sum = 0.0;
		ks_ptr->reset = &kahan_reset;
		ks_ptr->add = &kahan_add;
		ks_ptr->get_sum = &kahan_get_sum;
		return ks_ptr;
	}

#endif

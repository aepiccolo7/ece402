/* 
 * File:   FIR_Filters.h
 * Author: Andy
 */

#ifndef FIR_FILTERS_H
#define	FIR_FILTERS_H


typedef struct{
	float len;
	float *prev;
	float *h;
}FIR_T;

FIR_T *init_fir(float *filter_coefs, int n_coef);

FIR_T *change_fir(float *filter_coefs_new, FIR_T *s);

float calc_fir(FIR_T *s, float x);

void destroy_fir(FIR_T *s);

#endif	/* FIR_FILTERS_H */


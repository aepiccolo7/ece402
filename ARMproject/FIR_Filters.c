
#include <stdlib.h>
#include "FIR_Filters.h"

FIR_T *init_fir(float *filter_coefs, int n_coef )
{
	int i;
	FIR_T *s;
	s = malloc(sizeof(FIR_T));
	s->len = n_coef;
	s->h= malloc(sizeof(float)*n_coef);

	for (i=0;i<n_coef;i++) {
		s->h[i]=filter_coefs[i];
	}

	s->prev = calloc(n_coef,sizeof(float));
	return (s);
}

FIR_T *change_fir(float *filter_coefs_new, FIR_T *s)
{
	int i;
	for (i=0; i<s->len; i++) {
		s->h[i]=filter_coefs_new[i];
	}
	return s;
}

float calc_fir(FIR_T *s, float x) {
	float y=0;
	int j;

	//move j into previous array slot
	for (j=s->len-1;j>0;j--) {
		s->prev[j] = s->prev[j-1];
	}

	s->prev[0] = x;

	//convolution
	for (j=0;j<s->len;j++) {
		y+= s->prev[j] * s->h[j];
	}

	return y;

}

void destroy_fir(FIR_T *s)
{
	free(s->h);
	free(s->prev);
	free(s);
	return;
}





#include <stdlib.h>
#include "FIR_Filters.h"

FIR_T *init_fir(float *filter_coefs, int n_coef )
{
	int i;
	FIR_T *s;
	s = malloc(sizeof(FIR_T));
	s->len = n_coef;
	s->h= malloc(sizeof(int)*n_coef);

	for(i=0;i<n_coef;i++)
	{
		s->h[i]=filter_coefs[i];
	}

	s->prev = calloc(n_coef,sizeof(int));
	return (s);
}

float calc_fir(FIR_T *s, float x) {
	float y=0;
	int j,k;

	//move j into previous array slot
	for(j=s->len-1;j>0;j--)
	{
		s->prev[j] = s->prev[j-1];
	}

	s->prev[0] = x;

	//convolution
	for(j=0;j<s->len;j++)
	{
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




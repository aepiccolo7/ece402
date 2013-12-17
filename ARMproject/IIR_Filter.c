#include <stdio.h>
#include <stdlib.h>
#include "IIR_Filter.h"

IIR_T *init_iir(int order, float b[], float a[])
{
	IIR_T *s;
	s = malloc(sizeof(IIR_T));
	s->order = order;
	s->a = malloc(order*sizeof(float));
	s->b = malloc(order*sizeof(float));
	s->previous = calloc(order, sizeof(float));

	int i=0;
	for (i=0; i<order; i++) {
		s->a[i] = a[i];
		s->b[i] = b[i];
	}
	return s;
}

IIR_T *change_iir(float b[], float a[], IIR_T *s)
{
	int i=0;
	for (i=0; i<s->order; i++) {
		s->a[i] = a[i];
		s->b[i] = b[i];
	}
	return s;
}

float calc_iir(IIR_T *s, float x)
{
	float y=0;
	int j;

	for (j=s->order-1; j>0; j--){
		s->previous[j] = s->previous[j-1];
	}
	s->previous[0] = s->a[0]*x;
	for (j=1; j<s->order; j++) {
		s->previous[0] -= s->a[j] * s->previous[j];
	}

	for (j=0; j<s->order; j++) {
		y += s->b[j] * s->previous[j];
	}

	return y;

}


void destroy_iir(IIR_T *s)
{
	free(s->previous);
	free(s->a);
	free(s->b);
}


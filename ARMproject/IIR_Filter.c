#include <stdio.h>
#include <stdlib.h>
#include "IIR_Filter.h"

//Function to allocate memory and initialize an IIR filter structure
IIR_T *init_iir(int order, float b[], float a[])
{
	//allocate memory for the struct
	IIR_T *s;
	s = malloc(sizeof(IIR_T));

	//store order
	s->order = order;

	//allocate memory for numerator and denominator arrays
	s->a = malloc(order*sizeof(float));
	s->b = malloc(order*sizeof(float));

	//allocate memory for previous values array
	s->previous = calloc(order, sizeof(float));

	//store filter coefficients in the struct's arrays
	int i=0;
	for (i=0; i<order; i++) {
		s->a[i] = a[i];
		s->b[i] = b[i];
	}
	return s;
}

//Function to change the filter coefficients in an already initialized structure
IIR_T *change_iir(float b[], float a[], IIR_T *s)
{
	//reassign array values to new coefs
	int i=0;
	for (i=0; i<s->order; i++) {
		s->a[i] = a[i];
		s->b[i] = b[i];
	}
	return s;
}

//Function to calculate the output value based on filter coefs and previous values stored
//	in the IIR_T filter struct
float calc_iir(IIR_T *s, float x)
{
	float y=0;
	int j;

	//shift previous values
	for (j=s->order-1; j>0; j--){
		s->previous[j] = s->previous[j-1];
	}

	//assign current value
	s->previous[0] = s->a[0]*x;

	//subtract out denominator coefficient times latched value
	for (j=1; j<s->order; j++) {
		s->previous[0] -= s->a[j] * s->previous[j];
	}

	//add in numerator times latched value
	for (j=0; j<s->order; j++) {
		y += s->b[j] * s->previous[j];
	}

	//return result
	return y;

}

//Function to deallocate memory used by struct
void destroy_iir(IIR_T *s)
{
	free(s->previous);
	free(s->a);
	free(s->b);
}


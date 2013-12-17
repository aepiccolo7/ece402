
#include <stdlib.h>
#include "FIR_Filters.h"

//Function to Allocate memory for an FIR filter structure
FIR_T *init_fir(float *filter_coefs, int n_coef )
{
	int i;
	FIR_T *s;

	//allocate structure memory
	s = malloc(sizeof(FIR_T));

	//store length
	s->len = n_coef;

	//allocate array for coefficients
	s->h= malloc(sizeof(float)*n_coef);

	//store filter coefficients in h
	for (i=0;i<n_coef;i++) {
		s->h[i]=filter_coefs[i];
	}

	//allocate memory for previous values
	s->prev = calloc(n_coef,sizeof(float));
	return (s);
}

//Function to change the filter coefficients in the FIR structure
FIR_T *change_fir(float *filter_coefs_new, FIR_T *s)
{
	int i;

	//change each value
	for (i=0; i<s->len; i++) {
		s->h[i]=filter_coefs_new[i];
	}
	return s;
}

//Function to calculate the output value based on current input and previous
//	values stored in the structure based on filter coefficients.
float calc_fir(FIR_T *s, float x) {
	float y=0;
	int j;

	//move j into previous array slot
	for (j=s->len-1;j>0;j--) {
		s->prev[j] = s->prev[j-1];
	}

	//insert current value
	s->prev[0] = x;

	//convolution
	for (j=0;j<s->len;j++) {
		y+= s->prev[j] * s->h[j];
	}

	return y;

}

//Function to unallocate memory
void destroy_fir(FIR_T *s)
{
	free(s->h);
	free(s->prev);
	free(s);
	return;
}




#include <stdio.h>
#include <stdlib.h>
#include "FIR_Filters.h"

int main(int argc, char* argv[])
{
	float a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	float h[5] = {1, 5, 3, 2, 5};
	float b[14];

	int i;
	
	FIR_T *filter = init_fir(h, 5);
	
	for  (i=0; i< 14; i++) {
		b[i] = calc_fir(filter, a[i]);
		printf ("%f ", b[i]);
	}
	printf("\n");
	return 0;

}

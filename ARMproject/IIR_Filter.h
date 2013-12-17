#ifndef IIR_FILTER_H

#define IIR_FILTER_H


typedef struct{
  int order;
  float *previous;
  float *a; //denominator
  float *b; //numerator
}IIR_T;

IIR_T *init_iir(int order, float b[], float a[]);

IIR_T *change_iir(float b[], float a[], IIR_T *s);

float calc_iir(IIR_T *s, float x);

void destory_biquad(IIR_T *s);

#endif

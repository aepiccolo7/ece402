/* 
 * File:   main.c
 * Author: Andy
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include <htc.h>
#include "init.h"
#include "dsping.h"

    __CONFIG(FOSC_INTRC_CLKOUT & WDTE_ON & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & IESO_ON & FCMEN_ON & LVP_ON);
    __CONFIG(BOR4V_BOR40V & WRT_OFF);

int main(int argc, char** argv) {
    OSCCONbits.IRFC = 0b111;
    int data;
    int i=0, j=0;
    int lowpass, highpass;
    float * input, temp, output, filter1, filter2;
    int block = 100;				//block size
    int filterlength = 100; 		//order of filters

    init_ADC();
    init_DAC();

    TRISA = 0x00;
    while(1){
        give_dac_sample(get_adc_sample());
    }
    return (EXIT_SUCCESS);
}



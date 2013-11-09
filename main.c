/* 
 * File:   main.c
 * Author: Andy
 *
 * Created on November 7, 2013, 8:05 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include "FIR_Filters.h"
#include "init.h"
/*
 * 
 */
int main(int argc, char** argv) {
    TRISA = 0b00011100;
    int data;
    int i=0, j=0;
    float lowpass, highpass;
    float * input, temp, output, filter1, filter2;
    int block = 100;				//block size
    int filterlength = 100; 		//order of filters

    init_ADC();				//initialize everything

    while(1) {
        data = SPI1BUF;
        if (data < 0x7FFFFFFF) {
            PORTA &= ~(0x1C);
        } else {
            PORTA |= 0x1C;
        }
    }
    /*init_DAC();
    set_block_size(block);
    init_user_input();

    input = (float *)malloc(sizeof(float)*block);	//set up needed arrays
    temp = (float *)malloc(sizeof(float)*block);
    output = (float *)malloc(sizeof(float)*block);
    filter1 = (float *)malloc(sizeof(float)*filterlength);
    filter2  = (float *)malloc(sizeof(float)*filterlength);
    FIR_T *filterLP, filterHP;

    while (1) {
    	if (i%1000 == 0) {	//only once every 1000 times check
            lowpass = get_lowpass_cutoff();		//user input and change filter
            highpass = get_highpass_cutoff();		//cutoffs accordingly

            //add error checking
            calc_lowpass_filter(lowpass, filter1);	//change filter coefficients according
            calc_highpass_filter(highpass, filter2);	// to new cutoffs
            filterLP = init_fir(filter1, filterlength);
            filterHP = init_fir(filter2, filterlength);
            i=0;					//reset counter
    	}
	get_block(input);	//get input block from ADC
        for (j=0; j<block; j++) {
            temp = calc_fir(filterLP, input[j]);
            output = calc_fir(filterHP, temp);
        }
        put_block(output);				//stream block to DAC
    i++;						//increment counter
    }
    return (EXIT_SUCCESS);
    */
}


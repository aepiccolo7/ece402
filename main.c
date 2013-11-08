/* 
 * File:   main.c
 * Author: Andy
 *
 * Created on November 7, 2013, 8:05 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>

/*
 * 
 */
int main(int argc, char** argv) {
    TRISA = 0xF0;
    PORTA = 0x0F;
    int i=0;
    float lowpass, highpass;
    float * input, temp, output, filter1, filter2;
    int block = 100;									//block size
    int filterlength = 100;								//order of filters

    init_ADC();											//initialize everything
    init_DAC();
    set_block_size(block);
    init_user_input();

    input = (float *)malloc(sizeof(float)*block);		//set up needed arrays
    temp = (float *)malloc(sizeof(float)*block);
    output = (float *)malloc(sizeof(float)*block);
    filter1 = (float *)malloc(sizeof(float)*filterlength);
    filter2  = (float *)malloc(sizeof(float)*filterlength);

    while (1) {
    	if (i%1000 == 0) {								//only once every 1000 times check
            lowpass = get_lowpass_cutoff();				//   user input and change filter
            highpass = get_highpass_cutoff();			//	 cutoffs accordingly
            //add error checking
            calc_lowpass_filter(lowpass, filter1);		//change filter coefficients according
            calc_highpass_filter(highpass, filter2);	//	 to new cutoffs
            i=0;										//reset counter
    	}
	get_block(input);								//get input block from ADC
	temp = convolve(input, filter1);				//calculate first filter outputs
	output = convolve(temp, filter2);				//calculate output values
	put_block(output);								//stream block to DAC
    i++;											//increment counter
    }
    return (EXIT_SUCCESS);
}


/* 
 * File:   main.c
 * Author: Andy
 *
 * Created on November 7, 2013, 8:05 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <plib.h>
#include "FIR_Filters.h"
#include "init.h"
/*
 * 
 */
int main(int argc, char** argv) {
    SYSTEMConfigPerformance(50000000);
    SYSTEMConfigPB(50000000);
    TRISA = 0b11100011;
    int data;
    int i=0, j=0;
    int lowpass, highpass;
    float * input, temp, output, filter1, filter2;
    int block = 100;				//block size
    int filterlength = 100; 		//order of filters

    init_ADC();				//initialize everything

    RPB2R = 0x0007;         //REFCLKO to RPB2R
    REFOCON = 0x00009000;   //Enable REFCLKO module
    RPB15R = 0x0005; //set OC1 for pin RPB15
    OC1CON = 0x0006; //set OC1 mode
    PR2 = 0x0001;    //period
    OC1RS = 0x0001;  //duty cycle
    T2CONSET = 0x8000; //turn on timer2
    OC1CONSET = 0x8000; //Enable OC1 module

    while(1) {
        lowpass = get_lowpass_cutoff()/100;
        if (lowpass == 0){
            PORTA = 0x00;
        } else if((lowpass >0) && (lowpass <=10)) {
            PORTA = 0x04;
        } else if((lowpass >10) && (lowpass <=20)) {
            PORTA = 0x08;
        } else if((lowpass >20) && (lowpass <=30)) {
            PORTA = 0x0C;
        } else if((lowpass >30) && (lowpass <=40)) {
            PORTA = 0x10;
        } else if((lowpass >40) && (lowpass <50)) {
            PORTA = 0x14;
        } else {
            PORTA = 0x18;
        }
       /*data = SPI1BUF;
        if (data > 0x00000000) {
            PORTA &= ~(0x04);
        } else {
            PORTA |= 0x04;
        }
        */
    }
    /*init_DAC();
    set_block_size(block);

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

int get_lowpass_cutoff(void)
{
    int value;
    AD1CON1 = 0x000000E0; //turn off ADC and use autosample
    AD1CON3 = 0x00000100; //set autosample time
    AD1CHS = 0x00090000; //AN9 channel

    AD1CON1SET = 0x8000; //turn on ADC
    while(!(AD1CON1 & 0x0001)); //wait for conversion to finish
    value = ADC1BUF0;    //get adc value
    if (((value/60)+10) > 50) {
        return 5000;
    } else {
        return 100*((value/60)+5);
    }  
}

int get_highpass_cutoff(void)
{
    int value;
    AD1CON1 = 0x000000E0; //turn off ADC and use autosample
    AD1CON3 = 0x00000100; //set autosample time
    AD1CHS = 0x000A0000; //AN11 channel

    AD1CON1SET = 0x8000; //turn on ADC
    while(!(AD1CON1 & 0x0001)); //wait for conversion to finish
    value = ADC1BUF0;    //get adc value
    if (((value/60)+10) >50) {
        return 5000;
    } else {
        return 100*((value/60)+5);
    }
}
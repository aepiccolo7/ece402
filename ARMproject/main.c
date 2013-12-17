#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "dsping.h"
#include "init.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "filter_coef.h"
#include "FIR_Filters.h"
#include "driverlib/adc.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

#define SW1 GPIO_PIN_4
#define SW2 GPIO_PIN_0

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_3|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);	//Set system clock frequency
	int filter_order = 15;
	int i, j;
	int count = 0;
	int active = 0;
	uint32_t ADC4Value[4];
	uint32_t ADC5Value[4];
	uint32_t ADCvalue;

	FIR_T *lp_filter = init_fir(h_lp[30], filter_order);		//allocate memory for filters
	FIR_T *hp_filter = init_fir(h_hp[1], filter_order);			//allocate memory for filters

	uint32_t sample_frequency = 50000;							//desired sample rate
	uint32_t sample_period = 30 + (SysCtlClockGet() / sample_frequency); //empirically measured sample rate of 49.3 ksps

	//initialize
	init_DAC();
	init_ADC();
	init_user_input();
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);	//pin for checking processor usage
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);		//enable peripheral for switches and LEDs
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE);	//enable LEDs
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1|SW2);		//enable switch inputs

	//set up timer to count for sample period
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	TimerEnable(TIMER0_BASE, TIMER_A);


	while(1){
		TimerLoadSet(TIMER0_BASE, TIMER_A, sample_period);	//load timer value
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10);  	//check processor time usage

		//check if switch 1 is pushed
		if (!GPIOPinRead(GPIO_PORTF_BASE, SW1)) {
			active = ~active;
			while(!GPIOPinRead(GPIO_PORTF_BASE, SW1));
		}

		//every 1000th time through loop, adjust filter coefs
		if (count == 0) {
			//get ADC value 4 times and average it
			ADCIntClear(ADC0_BASE, 1);
			ADCProcessorTrigger(ADC0_BASE, 1);
			while(!ADCIntStatus(ADC0_BASE, 1, false));
			ADCSequenceDataGet(ADC0_BASE, 1, ADC4Value);
			ADCvalue = ((ADC4Value[0] + ADC4Value[1] + ADC4Value[2] + ADC4Value[3] + 2)/4)/16;  //8 bit value

			//convert ADC value to cutoff array value and change coefs
			if (ADCvalue > 245) ADCvalue = 245;
			i = (ADCvalue/5) + 1;
			lp_filter = change_fir(h_lp[i], lp_filter);

			//get other ADC value 4 times and average it
			ADCIntClear(ADC0_BASE, 2);
			ADCProcessorTrigger(ADC0_BASE, 2);
			while(!ADCIntStatus(ADC0_BASE, 2, false));
			ADCSequenceDataGet(ADC0_BASE, 2, ADC5Value);
			ADCvalue = ((ADC5Value[0] + ADC5Value[1] + ADC5Value[2] + ADC5Value[3] + 2)/4)/16;  //8 bit value

			//convert ADC value to cutoff array value and change coefs

			if (ADCvalue > 245) ADCvalue = 245;
			j = (ADCvalue/5) + 1;
			hp_filter = change_fir(h_hp[j], hp_filter);
			count++;
		}

		//either filter on(led green) or filter off (led red)
		if (active) {
			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_GREEN);
			give_dac_sample(calc_fir(hp_filter, calc_fir(lp_filter, get_adc_sample()) - 127) + 127);	//lp/hp filter adc sample and stream to dac
		} else {
			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED);
			give_dac_sample(get_adc_sample());
		}
	 	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00); //check processor time usage

	 	//keep track of number of times through the loop
	 	if (count != 1000) count++;
	 	else count = 0;

	 	//wait for end of sample period
		while(TimerValueGet(TIMER0_BASE, TIMER_A) > 50);
	}
}

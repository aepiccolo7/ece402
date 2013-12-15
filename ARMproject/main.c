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
	SysCtlClockSet(SYSCTL_SYSDIV_3|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	int filter_order = 15;
	int i, j;
	int count = 0;
	int active = 0;
	uint32_t ADC4Value[4];
	uint32_t ADC5Value[4];
	uint32_t ADCvalue;

	FIR_T *lp_filter = init_fir(h_lp[30], filter_order);
	FIR_T *hp_filter = init_fir(h_hp[1], filter_order);

	uint32_t sample_frequency = 50000;
	uint32_t sample_period = 30 + (SysCtlClockGet() / sample_frequency); //49.3 ksps

	init_DAC();
	init_ADC();
	init_user_input();
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1|SW2);


	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1){
		TimerLoadSet(TIMER0_BASE, TIMER_A, sample_period);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10);  //check processor time usage
		if (!GPIOPinRead(GPIO_PORTF_BASE, SW1)) {
			active = ~active;
			while(!GPIOPinRead(GPIO_PORTF_BASE, SW1));
		}


		//every 1000th time through loop, adjust filter coefs
		if (count == 0) {
			ADCIntClear(ADC0_BASE, 1);
			ADCProcessorTrigger(ADC0_BASE, 1);
			while(!ADCIntStatus(ADC0_BASE, 1, false));
			ADCSequenceDataGet(ADC0_BASE, 1, ADC4Value);
			ADCvalue = ((ADC4Value[0] + ADC4Value[1] + ADC4Value[2] + ADC4Value[3] + 2)/4)/16;  //8 bit value
			if (ADCvalue > 245) ADCvalue = 245;
			i = (ADCvalue/5) + 1;
			//lp_filter = change_fir(h_lp[i], lp_filter);

			ADCIntClear(ADC0_BASE, 2);
			ADCProcessorTrigger(ADC0_BASE, 2);
			while(!ADCIntStatus(ADC0_BASE, 2, false));
			ADCSequenceDataGet(ADC0_BASE, 2, ADC5Value);
			ADCvalue = ((ADC5Value[0] + ADC5Value[1] + ADC5Value[2] + ADC5Value[3] + 2)/4)/16;  //8 bit value
			if (ADCvalue > 245) ADCvalue = 245;
			j = (ADCvalue/5) + 1;
			//hp_filter = change_fir(h_hp[j], hp_filter);
			count++;
		}
		if (active) {
			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_GREEN);
			give_dac_sample(calc_fir(hp_filter, calc_fir(lp_filter, get_adc_sample()) - 127) + 127);	//lp/hp filter adc sample and stream to dac
		} else {
			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED);
			give_dac_sample(get_adc_sample());
		}
	 	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00); //check processor time usage
	 	if (count != 1000) count++;
	 	else count = 0;
		while(TimerValueGet(TIMER0_BASE, TIMER_A) > 50);
	}
}

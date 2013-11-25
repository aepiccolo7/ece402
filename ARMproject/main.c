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

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_3|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	int filter_order = 5, i, j;
	uint8_t count = 0;
	uint32_t ADC4Value[4];
	uint32_t ADC5Value[4];
	uint32_t ADCvalue;

	FIR_T *lp_filter = init_fir(h_lp[1], filter_order);
	FIR_T *hp_filter = init_fir(h_hp[1], filter_order);

	uint32_t sample_frequency = 50000;
	uint32_t sample_period = 30 + (SysCtlClockGet() / sample_frequency); //49.309 ksps

	init_DAC();
	init_ADC();
	init_user_input();
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1){
		TimerLoadSet(TIMER0_BASE, TIMER_A, sample_period);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10);
		if (count == 0) {
			ADCIntClear(ADC0_BASE, 1);
			ADCProcessorTrigger(ADC0_BASE, 1);
			while(!ADCIntStatus(ADC0_BASE, 1, false));
			ADCSequenceDataGet(ADC0_BASE, 1, ADC4Value);
			ADCvalue = ((ADC4Value[0] + ADC4Value[1] + ADC4Value[2] + ADC4Value[3] + 2)/4)/16;  //8 bit value
			if (ADCvalue > 245) ADCvalue = 245;
			i = (ADCvalue/5) + 1;
			lp_filter = change_fir(h_lp[i], lp_filter);

			ADCIntClear(ADC0_BASE, 2);
			ADCProcessorTrigger(ADC0_BASE, 2);
			while(!ADCIntStatus(ADC0_BASE, 2, false));
			ADCSequenceDataGet(ADC0_BASE, 2, ADC5Value);
			ADCvalue = ((ADC5Value[0] + ADC5Value[1] + ADC5Value[2] + ADC5Value[3] + 2)/4)/16;  //8 bit value
			if (ADCvalue > 245) ADCvalue = 245;
			j = (ADCvalue/5) + 1;
			hp_filter = change_fir(h_hp[j], hp_filter);
			count++;
		}
	 	give_dac_sample(calc_fir(hp_filter, calc_fir(lp_filter, get_adc_sample()) - 127) + 127);
	 	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
	 	if (count != 100) count++;
	 	else count = 0;
		while(TimerValueGet(TIMER0_BASE, TIMER_A) > 50);
	}
}

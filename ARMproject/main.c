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

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_3|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //system clock 40 MHz
	init_DAC();
	init_ADC();
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
	FIR_T *lp_filter = init_fir(h_lp[10], 5);
	FIR_T *hp_filter = init_fir(h_hp[10], 5);

	uint32_t sample_frequency = 50000;
	uint32_t sample_period = 30 + (SysCtlClockGet() / sample_frequency); //49.164 ksps
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	TimerEnable(TIMER0_BASE, TIMER_A);
	while(1){
		TimerLoadSet(TIMER0_BASE, TIMER_A, sample_period);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10);
	 	give_dac_sample(calc_fir(hp_filter, calc_fir(lp_filter, get_adc_sample()) - 127) + 127);
	 	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
		while(TimerValueGet(TIMER0_BASE, TIMER_A) > 50);
	}
}

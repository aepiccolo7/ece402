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


int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //system clock 40 MHz
	//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_7);
	init_DAC();
	init_ADC();

	uint32_t sample_frequency = 50000;
	uint32_t sample_period = 30 + (SysCtlClockGet() / sample_frequency); //49.8127 ksps
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
	TimerEnable(TIMER0_BASE, TIMER_A);
	while(1){
		TimerLoadSet(TIMER0_BASE, TIMER_A, sample_period);
	 	give_dac_sample(get_adc_sample());
		while(TimerValueGet(TIMER0_BASE, TIMER_A) > 50);
	}
}

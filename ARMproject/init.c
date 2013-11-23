#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

void init_ADC(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //control lines
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); //bits 0-5 data lines
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); //bits 6-7 data lines
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);	//RDbar control lines
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);  //INTbar control line
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5); //data input bits 0-5
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_7);	//data input bits 6-7
}

void init_DAC(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	//data lines
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//control lines
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7); //8 data pins
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5); //1 control line
}

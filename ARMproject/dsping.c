#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

//Function that returns the value of the ADC
uint8_t get_adc_sample(void)
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00); // pull RDbar low
	while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)); //wait till done
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x40); //pull RDbar high
	uint8_t data = (uint8_t)GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5); //compute value
	data |= ((uint8_t)GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_7));
    return data;
}

//Function that streams the value passed to it to the DAC
void give_dac_sample(uint8_t sample)
{
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x20);	//pull wr1 bar high
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, sample);		//change dac data
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);	//pull wr1 bar low to update dac output
}

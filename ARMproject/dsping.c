#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
/*
int get_adc_sample(void)
{
    PORTB &= (~0b00000100);
    while(PORTB & 0b00000010);
    PORTB |= 0b00000100;
    return PORTD;
}
*/
void give_dac_sample(uint8_t sample)
{
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x01);	//pull wr1 bar high
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, sample);		//change dac data
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x00);	//pull wr1 bar low to update dac output
}

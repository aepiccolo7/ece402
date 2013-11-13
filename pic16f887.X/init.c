#include <pic16f887.h>

void init_ADC(void)
{
    TRISB = 0xFA;
    PORTB = 0xFE;
}

void init_DAC(void)
{
    TRISC = 0x00;
    TRISB &= ~0b00010000;
}
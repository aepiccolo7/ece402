#include <pic16f887.h>

int get_adc_sample(void)
{
    PORTB &= (~0b00000100);
    while(PORTB & 0b00000010);
    PORTB |= 0b00000100;
    return PORTD;
}

void give_dac_sample(int sample)
{
    PORTB |= 0b00010000;
    PORTC = sample;
    PORTB &= ~0b00010000;
}
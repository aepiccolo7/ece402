#include <p32xxxx.h>
#include <plib.h>


void init_ADC(void)
{
    SDI1R = 0x00;   //select SDI1 for RPA1 = pin 3
    SS1R = 0x01;    //select SS(bar)1 for RPB3 = pin 7
    SPI1BRG = 0; //set baud rate
    SPI1CON = 0x00001CE0;   //configure SPI1
    SPI1CON2 = 0x00000188;
    SPI1CON |= 0x00008000;  //turn on SPI1
}

void init_DAC(void)
{

}


#include "delay.h"

void Delay_us(unsigned int us)
{
    unsigned int i;

    for (i=0; i<us; i++)
    {
        _nop_();
        _nop_();
        _nop_();
		_nop_();
    }
}

void Delay_ms(unsigned int ms)
{
    unsigned int i;
    for(i=0; i<ms; i++)
    {
        Delay_us(1000);
    }
}
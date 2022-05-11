#include "sysconfig.h"

void Port_IO_Init()
{
	//set P4[1] to push_pull model
    P74OUT |= 0x01;
	
	//enable crossbar
    XBR2 |= 0x40;

    //close the alam P4.1
    P4 &= 0xfd;
	
	//uart1,int1
	XBR1 |= 0x14;
	XBR2 |= 0x44;
}

void Oscillator_Init()
{
    uint16_t i = 0;
    OSCXCN = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN = 0x08;
}

void Timer0_Init(void)
{
    // Config Timer0 16bit timer
    TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = TIMER0_RELOAD_HIGH;           /// Reinit Timer0 High register
	TL0 = TIMER0_RELOAD_LOW;
	ET0 = 1;                            /// Timer0 interrupt enabled
	TR0 = 1;
}

void Interrupt_Init()
{
	EA = 1;
}

void Watchdog_Init()
{
    //disable watchdog
    EA = 0;
    WDTCN = 0xde;
    WDTCN = 0xad;
    EA = 1;
}

void SysConfig(void)
{
    Watchdog_Init();
    Oscillator_Init();
    Port_IO_Init();
	Timer0_Init();
	Interrupt_Init();

}
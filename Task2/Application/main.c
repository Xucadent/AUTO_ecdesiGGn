#include "sys.h"
#include "sysconfig.h"
#include "gui.h"
#include "control.h"

void main()
{
    SysConfig();
	KeyInit();
    LCDInit();
    DigitsInit();
    ADC0_Init();
    ADC0_Enable(1);
    DAC0_Init();
    Graphics_Init();
    Control_Init();
    
    GUI_Display_Boot();
    DAC0_Output(0);

    while (1)
    {
        Control_Task();
		Delay_ms(10);			
    }
    
}

void Interrupt_Timer0(void) interrupt 1
{
    TH0 = TIMER0_RELOAD_HIGH;
	TL0 = TIMER0_RELOAD_LOW;
    KeyDetect();
    DigitsScan();
}
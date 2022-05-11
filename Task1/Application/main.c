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
    GUIInit();
    Control_Init();
    
    GUI_Display_Boot();

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
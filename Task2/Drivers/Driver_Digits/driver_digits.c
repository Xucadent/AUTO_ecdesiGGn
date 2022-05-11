#include "driver_digits.h"

int16_t xdata DigChar[3][4];

void Led_Port_Init()
{
    //set to push_pull model
    P74OUT &= 0xFF;

    //enable crossbar
    XBR2 |= 0x40;
}

void DigitsInit(void)
{
    Led_Port_Init();
	P5 |= 0xFF;
	P6 |= 0x0F;
	P7 = 0xFF;
}

void SetDisplayChar(uint8_t chr) 
{
	switch(chr)
	{
		case '0': P7=0xC0; break;
		case '1': P7=0xF9; break;
		case '2': P7=0xA4; break;
		case '3': P7=0xB0; break;
		case '4': P7=0x99; break;
		case '5': P7=0x92; break;
		case '6': P7=0x82; break;
		case '7': P7=0xF8; break;
		case '8': P7=0x80; break;
		case '9': P7=0x90; break;
		case 'A': P7=0x88; break;
		case 'B': P7=0x80; break;
		case 'C': P7=0xC6; break;
		case 'D': P7=0xA1; break;
		case 'E': P7=0x86; break;
		case 'F': P7=0x8E; break;
		case 'L': P7=0xC7; break;
		case 'P': P7=0x8C; break;
		case 'R': P7=0x88; break;
		case 'b': P7=0x83; break;
		case 'c': P7=0xA7; break;
		case 'h': P7=0x8B; break;
		case 'i': P7=0xFB; break;
		case 'n': P7=0xAB; break;
		case 't': P7=0x87; break;
		case 'r': P7=0xAF; break;
		case 'v': P7=0xE3; break;
		case '-': P7=0xBF; break;
		case ' ': P7=0xFF; break;
		default: P7=0xFF; break;
	}
}

void SetDisplayDigit(uint8_t digit)
{
	switch(digit)
	{			
		case 0: P5 = 0xFE; P6 = 0xFF; break; 
		case 1: P5 = 0xFD; P6 = 0xFF; break; 
		case 2: P5 = 0xFB; P6 = 0xFF; break; 
		case 3: P5 = 0xF7; P6 = 0xFF; break; 
		case 4: P5 = 0xEF; P6 = 0xFF; break; 
		case 5: P5 = 0xDF; P6 = 0xFF; break; 
		case 6: P5 = 0xBF; P6 = 0xFF; break; 
		case 7: P5 = 0x7F; P6 = 0xFF; break; 
		
		case 8: P6 = 0xFE; P5 = 0xFF; break; 
		case 9:P6 = 0xFD; P5 = 0xFF; break; 
		case 10:P6 = 0xFB; P5 = 0xFF; break; 
		case 11:P6 = 0xF7; P5 = 0xFF; break;
	}
}

void DigitsScan(void)
{
    static uint8_t digit = 0;
    P7 = 0xFF;
    SetDisplayDigit(digit);
    SetDisplayChar(DigChar[digit/4][digit%4]);
    digit = (digit >= 11) ? 0 : (digit+1);
}

void DigitsDisplayStr(uint8_t digitnum, char str[])
{
    uint8_t cur;
    for(cur=0; str[cur] != 0; cur++)
    {
        if(cur < 4)
            DigChar[digitnum][cur] = str[cur];
    }
}

void DigitsDisplayInt(uint8_t digitnum, int16_t num)
{
	uint8_t dispchar[4] = "    ";
	uint8_t cur;
	if(num)
	{
		for(cur=0; num != 0; cur++)
		{
			dispchar[3-cur] = num % 10 + '0';
			num = num / 10;
		}
	}
	else
	{
		dispchar[3] = '0';
	}
	
	DigitsDisplayStr(digitnum, dispchar);
}

void DigitsDisplayFloat(uint8_t digitnum, float num)
{

}
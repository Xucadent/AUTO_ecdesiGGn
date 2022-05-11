#include "gui.h"

uint8_t xdata Graphics_Update_Flag = 0;
uint8_t xdata Graphics_BufNum = 0;
uint8_t xdata Graphics_Buffer[2][1024];

void Graphics_Init(void)
{
    uint16_t xdata cur;
    for(cur=0; cur<1024; cur++)
    {
        Graphics_Buffer[Graphics_BufNum][cur] = 0x00;
    }
}

void Graphics_Clear(void)
{
    uint16_t xdata cur;
    for(cur=0; cur<1024; cur++)
    {
        Graphics_Buffer[0][cur] = 0x00;
        Graphics_Buffer[1][cur] = 0x00;
    }
}

void Graphics_Alter_Point(uint8_t x, uint8_t y, uint8_t color)
{
    uint16_t xdata byte = y * 16 + x / 8;
    Graphics_Update_Flag = 1;
    if(color)
    {
        Graphics_Buffer[Graphics_BufNum][byte] |= PIXELSETMASK(x % 8);
    }
    else
    {
        Graphics_Buffer[Graphics_BufNum][byte] &= PIXELRESETMASK(x % 8);
    }
}

void Graphics_Show_Curve(uint16_t value[], uint8_t len)
{
    uint16_t xdata x, y;
    Graphics_Init();
    for (x=0; x<len; x++)
    {
        y = 47 - value[x] * 47 / 100;
        Graphics_Alter_Point(x, y, 1);
    }
    GUI_Display_Update();  
}

void GUI_Display_Boot(void)
{
    LCDWriteStr(0, 0, "   Helicopter   ");
    LCDWriteStr(1, 0, " Control System ");
    LCDWriteStr(2, 0, "  Automation91  ");
    LCDWriteStr(3, 0, "  Ye Guangyuan  ");

    DigitsDisplayStr(0, "----");
    DigitsDisplayStr(1, "----");
    DigitsDisplayStr(2, "----");
    
    Delay_ms(1000);
    LCDClearString();
    GUI_Display_Main();
    LCDDispCursor(0, 1);
}

void GUI_Display_Main(void)
{
    LCDClearString();
    LCDWriteStr(0, 0, " Calibration    ");
    LCDWriteStr(1, 0, " PID Parameters ");
    LCDWriteStr(2, 0, " Target Height  ");
    LCDWriteStr(3, 0, " Run Helicopter ");

    DigitsDisplayStr(0, "----");
    DigitsDisplayStr(1, "----");
    DigitsDisplayStr(2, "----");
}

void GUI_Display_Calibration(uint16_t min_feedback, uint16_t max_feedback)
{
    uint8_t xdata str[17] = "                ";
    
    LCDClearString();
    LCDWriteStr(0, 0, " Set Min Height ");
    sprintf(str, " MinFDB: %d", min_feedback);
    LCDWriteStr(1, 0, str);
    LCDWriteStr(2, 0, " Set Max Height ");
    sprintf(str, " MaxFDB: %d", max_feedback);
    LCDWriteStr(3, 0, str);

    DigitsDisplayStr(0, "----");
    DigitsDisplayStr(1, "----");
    DigitsDisplayStr(2, "----");
}

void GUI_Display_PIDSet(float kp, float ki, float kd)
{
    uint8_t xdata str[17] = "                ";

    LCDClearString();
    sprintf(str, " Kp: %.3f", kp);
    LCDWriteStr(0, 0, str);
    sprintf(str, " Ki: %.3f", ki);
    LCDWriteStr(1, 0, str);
    sprintf(str, " Kd: %.3f", kd);
    LCDWriteStr(2, 0, str);
}

void GUI_Display_TargetSet(uint16_t height)
{
    uint8_t xdata str[17] = "                ";

    LCDClearString();
    sprintf(str, " %d", height);
    LCDWriteStr(0, 0, " Set Target     ");
    LCDWriteStr(1, 0, " Height (0-100) ");
    LCDWriteStr(2, 0, str);
}

void GUI_Display_HCMotorCtrl(void)
{
    LCDClearString();
    LCDWriteStr(3,0,"Start Pause Stop");
}

void GUI_Digits_Display(uint16_t num1, uint16_t num2, uint8_t enable)
{
    if(enable)
        DigitsDisplayStr(0, "On");
    else
        DigitsDisplayStr(0, "Off");

    DigitsDisplayInt(1, num1);
    DigitsDisplayInt(2, num2);
}

void GUI_Display_Update(void)
{
    if(Graphics_Update_Flag)
    {
        LCDGraphicsBufShow(Graphics_Buffer[Graphics_BufNum], Graphics_Buffer[1-Graphics_BufNum]);
        Graphics_BufNum = 1 - Graphics_BufNum;
        Graphics_Update_Flag = 0;
    }
}
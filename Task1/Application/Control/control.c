#include "control.h"

control_t CtrlStruct;
graphics_cmd last_cmd = scatter_cmd;
graphics_status last_sta = select_cmd;

void Control_Init(void)
{
    CtrlStruct.keyinput[0] = KeyUp;
    CtrlStruct.keyinput[1] = KeyUp;
    CtrlStruct.keyinput[2] = KeyUp;
    CtrlStruct.P1.x = 0;
    CtrlStruct.P1.y = 0;
    CtrlStruct.P2.x = 0;
    CtrlStruct.P2.y = 0;
    CtrlStruct.cmd = scatter_cmd;
    CtrlStruct.sta = select_cmd;
}

void KeyInput_Task(void)
{
    uint8_t keynum;
    for(keynum=0; keynum<3; keynum++)
        CtrlStruct.keyinput[2-keynum] = GetKeyStatus(keynum); // 0-2替换成左中右

    if(CtrlStruct.sta == select_cmd)
    {
        if(CtrlStruct.keyinput[0] == ShortPress)
        {    
            CtrlStruct.sta = set_p1x;
            return;
        }
        else if(CtrlStruct.keyinput[0] == LongPress)
        {
            CtrlStruct.sta = select_cmd;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress)
        {
            CtrlStruct.cmd = (CtrlStruct.cmd + 1) % 4;
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress)
        {
            CtrlStruct.cmd = (CtrlStruct.cmd - 1) % 4;
            return;
        }
    }
    else if (CtrlStruct.sta == set_p1x)
    {
        if(CtrlStruct.keyinput[0] == ShortPress)
        {    
            CtrlStruct.sta = set_p1y;
            return;
        }
        else if(CtrlStruct.keyinput[0] == LongPress)
        {
            CtrlStruct.sta = select_cmd;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress || CtrlStruct.keyinput[1] == LongPress)
        {
            CURMOV_XP(CtrlStruct.P1.x);
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress || CtrlStruct.keyinput[2] == LongPress)
        {
            CURMOV_XN(CtrlStruct.P1.x);
            return;
        }
    }
    else if (CtrlStruct.sta == set_p1y)
    {
        if(CtrlStruct.keyinput[0] == ShortPress)
        {    
            if(CtrlStruct.cmd == scatter_cmd)
                CtrlStruct.sta = set_p1x;
            else if(CtrlStruct.cmd == circle_cmd)
            {
                CtrlStruct.sta = set_p2y;
                CtrlStruct.P2 = CtrlStruct.P1;
            }
            else
            {
                CtrlStruct.sta = set_p2x;
                CtrlStruct.P2 = CtrlStruct.P1;
            }
            return;
        }
        else if(CtrlStruct.keyinput[0] == LongPress)
        {
            CtrlStruct.sta = select_cmd;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress || CtrlStruct.keyinput[1] == LongPress)
        {
            CURMOV_YP(CtrlStruct.P1.y);
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress || CtrlStruct.keyinput[2] == LongPress)
        {
            CURMOV_YN(CtrlStruct.P1.y);
            return;
        }
    }
    else if (CtrlStruct.sta == set_p2x)
    {
        if(CtrlStruct.keyinput[0] == ShortPress)
        {    
            CtrlStruct.sta = set_p2y;
            return;
        }
        else if(CtrlStruct.keyinput[0] == LongPress)
        {
            CtrlStruct.sta = select_cmd;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress || CtrlStruct.keyinput[1] == LongPress)
        {
            CURMOV_XP(CtrlStruct.P2.x);
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress || CtrlStruct.keyinput[2] == LongPress)
        {
            CURMOV_XN(CtrlStruct.P2.x);
            return;
        }
    }
    else if (CtrlStruct.sta == set_p2y)
    {
        if(CtrlStruct.keyinput[0] == ShortPress)
        {    
            CtrlStruct.sta = set_p1x;
            CtrlStruct.P1 = CtrlStruct.P2;
            return;
        }
        else if(CtrlStruct.keyinput[0] == LongPress)
        {
            CtrlStruct.sta = select_cmd;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress || CtrlStruct.keyinput[1] == LongPress)
        {
            CURMOV_YP(CtrlStruct.P2.y);
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress || CtrlStruct.keyinput[2] == LongPress)
        {
            CURMOV_YN(CtrlStruct.P2.y);
            return;
        }
    }
}

void CursorDisp_Task(void)
{
    static Point cursor = {0,0};

    Graphics_Add_Cursor(cursor, 0);
    if(CtrlStruct.sta == set_p1x || CtrlStruct.sta == set_p1y)
    {
        cursor.x = CtrlStruct.P1.x;
        cursor.y = CtrlStruct.P1.y;
        Graphics_Add_Cursor(cursor, 1);
    }
    else if (CtrlStruct.sta == set_p2x || CtrlStruct.sta == set_p2y)
    {
        cursor.x = CtrlStruct.P2.x;
        cursor.y = CtrlStruct.P2.y;
        Graphics_Add_Cursor(cursor, 1);
    }
    
}

uint8_t code DigitsCMD[4][4] = {"Pt  ", "LinE", "Circ", "REct"};
uint8_t code DigitsPoint[4][4] = {"P1 h", "P1 v", "P2 h", "P2 v"};

void StatusDisp_Task(void)
{
    if(CtrlStruct.sta == 0 && last_sta != 0)
    {
        LCDLineClear(3);
        LCDWriteStr(3,0,"  OK    +    -  ");
    }
    else if(CtrlStruct.sta != 0 && last_sta == 0)
    {
        LCDLineClear(3);
        LCDWriteStr(3,0," NEXT   +    -  ");
    }
    
    if(CtrlStruct.sta == set_p1x || CtrlStruct.sta == set_p1y)
    {
		DigitsDisplayStr(0, DigitsPoint[CtrlStruct.sta-1]);
        DigitsDisplayInt(1, CtrlStruct.P1.x);
        DigitsDisplayInt(2, CtrlStruct.P1.y);
    }
    else if(CtrlStruct.sta == set_p2x || CtrlStruct.sta == set_p2y)
    {
		DigitsDisplayStr(0, DigitsPoint[CtrlStruct.sta-1]);
        DigitsDisplayInt(1, CtrlStruct.P2.x);
        DigitsDisplayInt(2, CtrlStruct.P2.y);
    }
    else
    {
		DigitsDisplayStr(0, DigitsCMD[CtrlStruct.cmd]);
        DigitsDisplayStr(1, "----");
        DigitsDisplayStr(2, "----");
    }
    
}

void Graphics_Task(void)
{
    if(last_sta != CtrlStruct.sta && CtrlStruct.sta == select_cmd)
    {
        switch (CtrlStruct.cmd)
        {
            case scatter_cmd: Graphics_Alter_Point(CtrlStruct.P1.x, CtrlStruct.P1.y, 1); break;
            case line_cmd: Graphics_Add_Line(CtrlStruct.P1, CtrlStruct.P2, 1); break;
            case circle_cmd: Graphics_Add_Circle(CtrlStruct.P1, CtrlStruct.P2.y - CtrlStruct.P1.y, 1); break;
            case rectangle_cmd: Graphics_Add_Rectangle(CtrlStruct.P1, CtrlStruct.P2, 1); break;
            default: break;
        }
        GUI_Display_Update();
    }
}

void Update_Last(void)
{
    last_sta = CtrlStruct.sta;
    last_cmd = CtrlStruct.cmd;
}

void Control_Task(void)
{
    KeyInput_Task();
    CursorDisp_Task();
    StatusDisp_Task();
    Graphics_Task();
    Update_Last();
}
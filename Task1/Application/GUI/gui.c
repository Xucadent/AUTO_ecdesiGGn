#include "gui.h"

uint8_t Graphics_Update_Flag = 0;
uint8_t xdata Graphics_Buffer[1024];

void Graphics_Alter_Point(uint8_t x, uint8_t y, uint8_t color)
{
    uint16_t byte = y * 16 + x / 8;
    Graphics_Update_Flag = 1;
    if(color)
    {
        Graphics_Buffer[byte] |= PIXELSETMASK(x % 8);
    }
    else
    {
        Graphics_Buffer[byte] &= PIXELRESETMASK(x % 8);
    }
}

void Graphics_Add_Line(Point head_pos, Point end_pos, uint8_t color)
{
    // Bresenham算法
    uint8_t x = head_pos.x;
    uint8_t y = head_pos.y;
    uint8_t dx = end_pos.x > head_pos.x ? end_pos.x - head_pos.x : head_pos.x - end_pos.x;
    uint8_t dy = end_pos.y > head_pos.y ? end_pos.y - head_pos.y : head_pos.y - end_pos.y;
    int8_t s1 = end_pos.x > x ? 1 : -1;
    int8_t s2 = end_pos.y > y ? 1 : -1;

    int16_t i, p;
	uint8_t interchange = 0;	// 默认不互换 dx、dy
	if (dy > dx)				// 当斜率大于 1 时，dx、dy 互换
	{
		uint8_t temp = dx;
		dx = dy;
		dy = temp;
		interchange = 1;
	}

	p = 2 * dy - dx;
	for(i = 0; i < dx; i++)
	{
		Graphics_Alter_Point(x, y, color);
		if (p >= 0)
		{
			if (!interchange)		// 当斜率 < 1 时，选取上下象素点
				y += s2;
			else					// 当斜率 > 1 时，选取左右象素点
				x += s1;
			p -= 2 * dx;
		}
		if (!interchange)
			x += s1;				// 当斜率 < 1 时，选取 x 为步长
		else
			y += s2;				// 当斜率 > 1 时，选取 y 为步长
		p += 2 * dy;
	}

}

void Graphics_Add_Circle(Point center, uint8_t r, uint8_t color)
{
    uint8_t x0 = center.x, y0 = center.y;
    int16_t a=0, b=r, di;
    di=3-2*r;             //判断下个点位置的标志
    while(a<=b)
    {
        Graphics_Alter_Point(x0-b,y0-a,color);             //3           
        Graphics_Alter_Point(x0+b,y0-a,color);             //0           
        Graphics_Alter_Point(x0-a,y0+b,color);             //1      
        Graphics_Alter_Point(x0-b,y0-a,color);             //7           
        Graphics_Alter_Point(x0-a,y0-b,color);             //2            
        Graphics_Alter_Point(x0+b,y0+a,color);             //4               
        Graphics_Alter_Point(x0+a,y0-b,color);             //5
        Graphics_Alter_Point(x0+a,y0+b,color);             //6
        Graphics_Alter_Point(x0-b,y0+a,color);            
        a++;
        /***使用Bresenham算法画圆**/     
        if(di<0)
            di += 10*a+6;
        else
        {
            di += 10+10*(a-b);   
            b--;
        }
        Graphics_Alter_Point(x0+a,y0+b,color);
    }
}

void Graphics_Add_Rectangle(Point cor1_pos, Point cor2_pos, uint8_t color)
{
    Point corner[4];
    corner[0] = cor1_pos;
    corner[1].x = cor2_pos.x; corner[1].y = cor1_pos.y;
    corner[2] = cor2_pos;
    corner[3].x = cor1_pos.x; corner[3].y = cor2_pos.y;

    Graphics_Add_Line(corner[0], corner[1], color);
    Graphics_Add_Line(corner[1], corner[2], color);
    Graphics_Add_Line(corner[2], corner[3], color);
    Graphics_Add_Line(corner[3], corner[0], color);
}

void Graphics_Add_Cursor(Point center, uint8_t color)
{
    uint16_t byte_num[3];
    uint8_t cursor_byte_data[4];
    uint8_t odd_flag = center.x / 8 % 2;
    byte_num[0] = (center.y-1) * 16 + center.x / 8;
    byte_num[1] = center.y * 16 + center.x / 8;
    byte_num[2] = (center.y+1) * 16 + center.x / 8;
    
    if(color)
    {
        cursor_byte_data[0] = Graphics_Buffer[byte_num[0]] | PIXELSETMASK(center.x % 8);
        cursor_byte_data[1] = Graphics_Buffer[byte_num[1]] | PIXELSETMASK(center.x % 8 - 1) | PIXELSETMASK(center.x % 8 + 1);
        cursor_byte_data[1] &= PIXELRESETMASK(center.x % 8);
        cursor_byte_data[2] = Graphics_Buffer[byte_num[2]] | PIXELSETMASK(center.x % 8);
    }
    else
    {
        cursor_byte_data[0] = Graphics_Buffer[byte_num[0]];
        cursor_byte_data[1] = Graphics_Buffer[byte_num[1]];
        cursor_byte_data[2] = Graphics_Buffer[byte_num[2]];
    }    

    if(odd_flag)
    {
        LCDSetByte(Graphics_Buffer[byte_num[0]-1], cursor_byte_data[0], byte_num[0]-1);
        LCDSetByte(Graphics_Buffer[byte_num[1]-1], cursor_byte_data[1], byte_num[1]-1);
        LCDSetByte(Graphics_Buffer[byte_num[2]-1], cursor_byte_data[2], byte_num[2]-1);
    }
    else
    {
        LCDSetByte(cursor_byte_data[0], Graphics_Buffer[byte_num[0]+1], byte_num[0]);
        LCDSetByte(cursor_byte_data[1], Graphics_Buffer[byte_num[1]+1], byte_num[1]);
        LCDSetByte(cursor_byte_data[2], Graphics_Buffer[byte_num[2]+1], byte_num[2]);
    }

    if(center.x % 8 == 0)
    {
        if(color)
            cursor_byte_data[3] = Graphics_Buffer[byte_num[1]-1] | PIXELSETMASK(7);
        else
            cursor_byte_data[3] = Graphics_Buffer[byte_num[1]-1];

        if(odd_flag)
            LCDSetByte(cursor_byte_data[3], cursor_byte_data[1], byte_num[1]-1);
        else
            LCDSetByte(Graphics_Buffer[byte_num[1]-2], cursor_byte_data[3], byte_num[1]-2);
    }
    else if (center.x % 8 == 7)
    {
        if(color)
            cursor_byte_data[3] = Graphics_Buffer[byte_num[1]+1] | PIXELSETMASK(0);
        else
            cursor_byte_data[3] = Graphics_Buffer[byte_num[1]+1];

        if(odd_flag)
            LCDSetByte(cursor_byte_data[3], Graphics_Buffer[byte_num[1]+2], byte_num[1]+1);
        else
            LCDSetByte(cursor_byte_data[1], cursor_byte_data[3], byte_num[1]);
    }
    
}

void GUIInit(void)
{
    uint16_t cur;
    for(cur=0; cur<1024; cur++)
        Graphics_Buffer[cur] = 0x00;
}

void GUI_Display_Boot(void)
{
    LCDWriteStr(1, 0, "  Pixel Paint   ");
    LCDWriteStr(2, 0, "  Automation91  ");
    LCDWriteStr(3, 0, "  Ye Guangyuan  ");

    DigitsDisplayStr(0, "----");
    DigitsDisplayStr(1, "----");
    DigitsDisplayStr(2, "----");
    
    Delay_ms(1000);
    LCDClear();
    LCDWriteStr(3,0,"  OK    +    -  ");
}

void GUI_Display_Update(void)
{
    if(Graphics_Update_Flag)
    {
        LCDImageShow(Graphics_Buffer);
        Graphics_Update_Flag = 0;
    }
}
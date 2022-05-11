#ifndef GUI_H
#define GUI_H

#include "sys.h"
#include "driver_lcd.h"
#include "driver_digits.h"
 
#define PIXELRESETMASK(n) (0xFF ^ (0x80>>n))
#define PIXELSETMASK(n) (0x80>>n)

typedef struct 
{   
    uint8_t x, y;    
}Point;


void Graphics_Alter_Point(uint8_t x, uint8_t y, uint8_t color);
void Graphics_Add_Line(Point head_pos, Point end_pos, uint8_t color);
void Graphics_Add_Circle(Point center, uint8_t r, uint8_t color);
void Graphics_Add_Rectangle(Point cor1_pos, Point cor2_pos, uint8_t color);
void Graphics_Add_Cursor(Point center, uint8_t color);
void GUIInit(void);
void GUI_Display_Boot(void);
void GUI_Display_Update(void);


#endif
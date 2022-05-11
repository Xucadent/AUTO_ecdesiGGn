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

void Graphics_Init(void);
void Graphics_Clear(void);
void Graphics_Alter_Point(uint8_t x, uint8_t y, uint8_t color);
void Graphics_Show_Curve(uint16_t value[], uint8_t len);
void GUI_Display_Update(void);


void GUI_Display_Boot(void);
void GUI_Display_Main(void);
void GUI_Display_Calibration(uint16_t min_feedback, uint16_t max_feedback);
void GUI_Display_PIDSet(float kp, float ki, float kd);
void GUI_Display_TargetSet(uint16_t height);
void GUI_Display_HCMotorCtrl(void);
void GUI_Digits_Display(uint16_t num1, uint16_t num2, uint8_t enable);



#endif
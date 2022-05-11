#ifndef DRIVER_LCD_H
#define DRIVER_LCD_H

#include "sys.h"
#include <stdio.h>

sbit SCLK=P1^3;
sbit SID=P1^4;
sbit CS=P1^5;

void LCDInit(void);
void LCDLineClear(uint8_t l);
void LCDClearString(void);
void LCDClearImage(void);
void LCDWriteChar(uint8_t row,uint8_t col,uint8_t ch);
void LCDWriteStr(uint8_t row,uint8_t col,uint8_t *puts);
void LCDImageShow(uint8_t *imagePtr);
void LCDGraphicsBufShow(uint8_t *GraphicsPtr, uint8_t *LastPtr);
void LCDSetByte(uint8_t byte_even, uint8_t byte_odd, uint16_t byte_num);
void LCDDispCursor(uint8_t row, uint8_t color);

#endif
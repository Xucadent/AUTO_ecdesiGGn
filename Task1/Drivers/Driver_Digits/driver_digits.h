#ifndef DRIVER_DIGITS_H
#define DRIVER_DIGITS_H

#include "sys.h"

void DigitsInit(void);
void DigitsScan(void);
void DigitsDisplayStr(uint8_t digitnum, char str[]);
void DigitsDisplayInt(uint8_t digitnum, int16_t num);
void DigitsDisplayFloat(uint8_t digitnum, float num);

#endif
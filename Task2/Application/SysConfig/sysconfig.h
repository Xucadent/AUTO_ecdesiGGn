#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include "sys.h"

#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long
#define int8_t char
#define int16_t int
#define int32_t long


#define LED_TOGGLE_RATE 1 // LED toggle rate in milliseconds    1ms

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

#define AUX1     TIMER_TICKS_PER_MS*LED_TOGGLE_RATE
#define AUX2     -AUX1
#define AUX3     AUX2&0x00FF
#define AUX4     ((AUX2&0xFF00)>>8)

#define TIMER0_RELOAD_HIGH       AUX4  // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW        AUX3  // Reload value for Timer0 low byte 

void SysConfig(void);

#endif
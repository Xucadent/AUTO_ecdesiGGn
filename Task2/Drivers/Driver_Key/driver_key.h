#ifndef DRIVER_KEY_H
#define DRIVER_KEY_H

#include "sys.h"

#define RESETMASK(n) (0xFF ^ (1<<n))
#define SETMASK(n) (1<<n)
#define KEYRESET(n) (P5 &= RESETMASK(n))
#define KEYSET(n) (P5 |= SETMASK(n))
#define KEYCOMM ((P0 & 0x08) >> 3)
#define KAQLEN 10

typedef enum
{
    KeyUp = 0,
    ShortPress = 1,
    LongPress = 2
}keysta_t;

typedef struct
{
    uint8_t keyact[KAQLEN];
    uint8_t head;
    uint8_t tail;
}keyactqueue_t;

void KeyInit(void);
void KeyDetect(void);
uint8_t GetKeyStatus(uint8_t keynum);


#endif
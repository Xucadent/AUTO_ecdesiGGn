#ifndef CONTROL_H
#define CONTROL_H

#include "driver_digits.h"
#include "driver_key.h"
#include "driver_lcd.h"
#include "gui.h"

#define CURMOV_XP(x) if(x < 127) ++x; else x=0;
#define CURMOV_XN(x) if(x > 0) --x; else x=127;
#define CURMOV_YP(y) if(y < 63) ++y; else y=0;
#define CURMOV_YN(y) if(y > 0) --y; else y=63;

typedef enum
{
    scatter_cmd = 0,
    line_cmd = 1,
    circle_cmd = 2,
    rectangle_cmd = 3
}graphics_cmd;

typedef enum
{
    select_cmd = 0,
    set_p1x = 1,
    set_p1y = 2, 
    set_p2x = 3,
    set_p2y = 4
}graphics_status;

typedef struct
{
    uint8_t keyinput[3];
    graphics_status sta;
    graphics_cmd cmd;
    Point P1, P2;
}control_t;

void Control_Init(void);
void Control_Task(void);

#endif
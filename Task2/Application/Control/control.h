#ifndef CONTROL_H
#define CONTROL_H

#include "driver_digits.h"
#include "driver_key.h"
#include "driver_lcd.h"
#include "driver_adda.h"
#include "gui.h"
#include "pid.h"

#define CURMOV_XP(x) if(x < 127) ++x; else x=0;
#define CURMOV_XN(x) if(x > 0) --x; else x=127;
#define CURMOV_YP(y) if(y < 63) ++y; else y=0;
#define CURMOV_YN(y) if(y > 0) --y; else y=63;

typedef enum
{
    sta_select_settings = 0,
    sta_calibration = 1,
    sta_set_pid = 2,
    sta_set_target = 3,
    sta_motor_ctrl = 4
}sysstatus_t;

typedef enum
{
    main_calibration = 0,
    main_set_pid = 1,
    main_set_target = 2,
    main_start = 3
}mainmenu_t;

typedef enum
{
    cali_select = 2,
    cali_setmin = 0,
    cali_setmax = 1
}calimenu_t;

typedef enum
{
    pid_select = 3,
    pid_setkp = 0,
    pid_setki = 1,
    pid_setkd = 2
}pidmenu_t;

typedef enum
{
    enable_motor = 0,
    disable_motor = 1
}motorstatus_t;

typedef struct
{
    uint8_t keyinput[3];
    uint8_t cursor;
    uint16_t target_height;
    uint16_t caliparam[2];
    sysstatus_t syssta;
    mainmenu_t mainmenusta;
    calimenu_t calimenusta;
    pidmenu_t pidmenusta;
    motorstatus_t motorsta;
    PID HCMotorPID;
}control_t;

void Control_Init(void);
void Control_Task(void);

#endif
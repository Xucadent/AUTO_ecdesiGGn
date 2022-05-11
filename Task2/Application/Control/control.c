#include "control.h"

control_t xdata CtrlStruct;
sysstatus_t xdata last_syssta;
uint8_t xdata last_cursor;
uint8_t xdata update_flag;
keysta_t xdata last_keysta[3];
motorstatus_t xdata last_motorsta;
uint16_t xdata height_buffer[128];
uint16_t xdata height_buffer_index = 0;
uint16_t xdata div_cnt = 0;

void Control_Init(void)
{
    for(height_buffer_index = 0; height_buffer_index < 128; height_buffer_index++)
    {
        height_buffer[height_buffer_index] = 0;
    }
    height_buffer_index = 0;

    CtrlStruct.keyinput[0] = KeyUp;
    CtrlStruct.keyinput[1] = KeyUp;
    CtrlStruct.keyinput[2] = KeyUp;
    
    CtrlStruct.cursor = 0;
    CtrlStruct.target_height = 0;
    CtrlStruct.syssta = sta_select_settings;
    CtrlStruct.mainmenusta = main_calibration;
    CtrlStruct.calimenusta = cali_select;
    CtrlStruct.pidmenusta = pid_select;
    CtrlStruct.motorsta = disable_motor;
    CtrlStruct.caliparam[0] = 0;
    CtrlStruct.caliparam[1] = 0;
    
    CtrlStruct.HCMotorPID.Kp = 0.6;
    CtrlStruct.HCMotorPID.Ki = 0.015;
    CtrlStruct.HCMotorPID.Kd = 0;
    CtrlStruct.HCMotorPID.InteMax = 20000;
    CtrlStruct.HCMotorPID.InteMin = -20000;
    CtrlStruct.HCMotorPID.OutMax = 32767;
    CtrlStruct.HCMotorPID.OutMin = 0;

    last_keysta[0] = KeyUp;
    last_keysta[1] = KeyUp;
    last_keysta[2] = KeyUp;
    last_syssta = sta_select_settings;
    last_cursor = 0;
    last_motorsta = disable_motor;
    update_flag = 0;
}

void KeyInput_Task(void)
{
    uint8_t xdata keynum;
    for(keynum=0; keynum<3; keynum++)
        CtrlStruct.keyinput[2-keynum] = GetKeyStatus(keynum); // 0-2替换成左中右

    if(CtrlStruct.syssta == sta_select_settings)
    {
        if(CtrlStruct.keyinput[0] == ShortPress)
        {    
            CtrlStruct.syssta = CtrlStruct.cursor + 1;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress)
        {
            CtrlStruct.cursor = (CtrlStruct.cursor + 1) % 4;
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress)
        {
            CtrlStruct.cursor = (CtrlStruct.cursor - 1) % 4;
            return;
        }
    }
    else if (CtrlStruct.syssta == sta_calibration)
    {
        if(CtrlStruct.keyinput[0] == ShortPress && CtrlStruct.calimenusta == cali_select)
        {    
            CtrlStruct.calimenusta = CtrlStruct.cursor == 0 ? cali_setmin : cali_setmax;
            return;
        }
        else if(CtrlStruct.keyinput[0] == LongPress && last_keysta[0] == KeyUp)
        {
            if(CtrlStruct.calimenusta == cali_select)
                CtrlStruct.syssta = sta_select_settings;
            else
                CtrlStruct.calimenusta = cali_select;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress || CtrlStruct.keyinput[1] == LongPress)
        {
            // if(CtrlStruct.calimenusta == cali_setmin)
            // {
            //     CtrlStruct.HCMotorPID.OutMin += 1;
            //     update_flag = 1;
            // }  
            // else if(CtrlStruct.calimenusta == cali_setmax)
            // {
            //     CtrlStruct.HCMotorPID.OutMax += 1;
            //     update_flag = 1;
            // } 
            // else
                CtrlStruct.cursor = (CtrlStruct.cursor == 0) ? 2 : 0;
            // return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress || CtrlStruct.keyinput[2] == LongPress)
        {
            // if(CtrlStruct.calimenusta == cali_setmin)
            // {
            //     CtrlStruct.HCMotorPID.OutMin -= 1;
            //     update_flag = 1;
            // }

            // else if(CtrlStruct.calimenusta == cali_setmax)
            // {
            //     CtrlStruct.HCMotorPID.OutMax -= 1;
            //     update_flag = 1;
            // }
            // else
                CtrlStruct.cursor = (CtrlStruct.cursor == 0) ? 2 : 0;
            // return;
        }
        if(CtrlStruct.calimenusta == cali_setmin)
        {
            CtrlStruct.caliparam[0] = ADC0_GetValue();
            update_flag = 1;
        }

        else if(CtrlStruct.calimenusta == cali_setmax)
        {
            CtrlStruct.caliparam[1] = ADC0_GetValue();
            update_flag = 1;
        }
    }
    else if (CtrlStruct.syssta == sta_set_pid)
    {
        if(CtrlStruct.keyinput[0] == ShortPress && CtrlStruct.pidmenusta == pid_select)
        {    
            CtrlStruct.pidmenusta = CtrlStruct.cursor;
            return;
        }
        else if(CtrlStruct.keyinput[0] == LongPress && last_keysta[0] == KeyUp)
        {
            if(CtrlStruct.pidmenusta == pid_select)
                CtrlStruct.syssta = sta_select_settings;
            else
                CtrlStruct.pidmenusta = pid_select;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress || CtrlStruct.keyinput[1] == LongPress)
        {
            if(CtrlStruct.pidmenusta == pid_setkp)
            {
                CtrlStruct.HCMotorPID.Kp += 0.01;
                update_flag = 1;
            }  
            else if(CtrlStruct.pidmenusta == pid_setki)
            {
                CtrlStruct.HCMotorPID.Ki += 0.01;
                update_flag = 1;
            } 
            else
                CtrlStruct.cursor = (CtrlStruct.cursor + 1) % 2;
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress || CtrlStruct.keyinput[2] == LongPress)
        {
            if(CtrlStruct.pidmenusta == pid_setkp)
            {
                CtrlStruct.HCMotorPID.Kp -= 0.01;
                update_flag = 1;
            }

            else if(CtrlStruct.pidmenusta == pid_setki)
            {
                CtrlStruct.HCMotorPID.Ki -= 0.01;
                update_flag = 1;
            }
            else
                CtrlStruct.cursor = (CtrlStruct.cursor - 1) % 3;  
            return;
        }
    }
    else if (CtrlStruct.syssta == sta_set_target)
    {
        if(CtrlStruct.keyinput[0] == LongPress && last_keysta[0] == KeyUp)
        {
            CtrlStruct.syssta = sta_select_settings;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress || CtrlStruct.keyinput[1] == LongPress)
        {
            CtrlStruct.target_height = CtrlStruct.target_height < 100 ? CtrlStruct.target_height + 1 : CtrlStruct.target_height;
            update_flag = 1;
            return;
        }
        if(CtrlStruct.keyinput[2] == ShortPress || CtrlStruct.keyinput[2] == LongPress)
        {
            CtrlStruct.target_height = CtrlStruct.target_height > 0 ? CtrlStruct.target_height - 1 : CtrlStruct.target_height;
            update_flag = 1;
            return;
        }
    }
    else if(CtrlStruct.syssta == sta_motor_ctrl)
    {
        if(CtrlStruct.keyinput[0] == ShortPress)
        {    
            CtrlStruct.motorsta = enable_motor;
            update_flag = 1;
            return;
        }

        if(CtrlStruct.keyinput[1] == ShortPress)
        {
            CtrlStruct.motorsta = disable_motor;
            update_flag = 1;
            return;
        }

        if(CtrlStruct.keyinput[2] == ShortPress)
        {
            CtrlStruct.motorsta = disable_motor;
            CtrlStruct.syssta = sta_select_settings;
            return;
        }
    }
}

void CursorDisp_Task(void)
{
    if(CtrlStruct.cursor != last_cursor)
    {
        if(CtrlStruct.syssta == sta_motor_ctrl)
        {
            LCDClearImage();
        }
        else
        {
            LCDDispCursor(last_cursor, 0);
            LCDDispCursor(CtrlStruct.cursor, 1);
        }
    }
}

void Clear_Height_Buffer(void)
{
    uint8_t i;
    for(i = 0; i < 128; i++)
    {
        height_buffer[i] = 0;
    }
    height_buffer_index = 0;
}

void InterfaceDisp_Task(void)
{
    PID xdata HCPID;
	HCPID = CtrlStruct.HCMotorPID;

    if(CtrlStruct.syssta != last_syssta || update_flag)
    {
        switch (CtrlStruct.syssta)
        {
            case sta_select_settings: GUI_Display_Main(); break;
            case sta_calibration: GUI_Display_Calibration(CtrlStruct.caliparam[0], CtrlStruct.caliparam[1]); break;
            case sta_set_pid: GUI_Display_PIDSet(HCPID.Kp, HCPID.Ki, HCPID.Kd); break;
            case sta_set_target: GUI_Display_TargetSet(CtrlStruct.target_height); break;
            case sta_motor_ctrl: GUI_Display_HCMotorCtrl(); break;
            default: break;
        }
        if(last_syssta == sta_motor_ctrl)
        {
            LCDClearImage();
            Graphics_Clear();
            Clear_Height_Buffer();
            PidClear(&CtrlStruct.HCMotorPID);
        }
            

        update_flag = 0;   
    }
    
}

void Update_Last(void)
{
    last_keysta[0] = CtrlStruct.keyinput[0];
    last_keysta[1] = CtrlStruct.keyinput[1];
    last_keysta[2] = CtrlStruct.keyinput[2];
    last_syssta = CtrlStruct.syssta;
    last_cursor = CtrlStruct.cursor;
    last_motorsta = CtrlStruct.motorsta;
}

void PID_Task(void)
{
    if(CtrlStruct.motorsta == enable_motor)
    {
        CtrlStruct.HCMotorPID.Ref = CtrlStruct.target_height * (CtrlStruct.caliparam[1] - CtrlStruct.caliparam[0]) / 100 + CtrlStruct.caliparam[0];
        CtrlStruct.HCMotorPID.Fdb = ADC0_GetValue();
        div_cnt = (div_cnt + 1) % 3;
        if(div_cnt == 0)
        {
            height_buffer[height_buffer_index] = (CtrlStruct.HCMotorPID.Fdb - CtrlStruct.caliparam[0]) * 100 /
						(CtrlStruct.caliparam[1] - CtrlStruct.caliparam[0]);
            height_buffer_index = (height_buffer_index + 1) % 128;
            
            Graphics_Show_Curve(height_buffer, 128);
            if(height_buffer_index == 127)
                Clear_Height_Buffer();
        }
        DigitsDisplayInt(0, CtrlStruct.HCMotorPID.Fdb);
        DigitsDisplayInt(1, CtrlStruct.HCMotorPID.Ref);
        DigitsDisplayInt(2, CtrlStruct.HCMotorPID.Out/10);
        PidCalc(&CtrlStruct.HCMotorPID);
        DAC0_Output(CtrlStruct.HCMotorPID.Out);
    }
    else
    {
        DAC0_Output(0);
    }
}

void Control_Task(void)
{
    KeyInput_Task();
    CursorDisp_Task();
    InterfaceDisp_Task();
    PID_Task();
    Update_Last();
}
#include "driver_adda.h"

/// ADC

void Timer3_Init_ADC0(int counts) 
{
    Timer3_Stop;                   // 清除 TMR3CN 中断标志位 2，定时器 3 禁止（重置所有状态）
    Timer3_Reload_Init = -counts;  // 使用 TMR3RL = -counts 重载定时器 3
//    Timer3_Reload_Init = 0x0000;  // 使用 TMR3RL = -counts 重载定时器 3
    Timer3_Count_Start;            // 设置 TMR3 计满溢出，立即装载
    Disable_Timer3;                // 清除 EIE2 标志位 0，禁止定时器 3 中断
    Timer3_Start;                  // 设定 TMR3CN 中断标志位 2，定时器 3 开启
}

void ADC0_Init(void)
{
    Timer3_Init_ADC0(SYSCLK / SAMPLERATE);
    ADC0_Ctr_Set;    // ADC0 控制设置，定时器 3 溢出启动跟踪，持续三个 SAR 时钟，然后进行 AD 转换，数据右对齐
    REF_Ctr_Set;     // 关闭温度传感器, 设定芯片上的 VREF
    ADC0_Channel_1;  // ADC0 使用 AIN1 通道
    ADC0_Clk_Set;    // 设置 ADC0 SAR 转换时钟周期为 2.5MHz
    ADC0_Gain;       // ADC0 增益设置，设定 ACD0 的增益为 1
    Enable_ADC0;     // 设定 EIE2 标志位 1，允许 ADC0 转换结束产生的中断请求
}

void ADC0_Enable(uint8_t flag)
{
    if (flag) 
        ADC0_Start;         // 设定 ADC0CN 允许位，ADC0 中断允许
    else
        ADC0_Stop;          // 清除 ADC0CN 允许位，ADC0 中断禁止
}

uint16_t xdata ADC_Value = 0;
void ADC0_ISR(void) interrupt 15 
{
    ADC0_IntFlag_Clear;      // 清除 ADC0CN 转换结束中断标志位，必须使用软件清零
    ADC_Value = ADC0H * 256 + ADC0L;  // 读 ADC0 中 AIN 通道的值
}

uint16_t ADC0_GetValue(void)
{
    return ADC_Value;
}

/// DAC
void DAC0_Init(void) 
{
    // REF0CN = 0x03;  // 使用内部VREF发生器
    DAC0_Ctr_Set;  // 打开 DAC0，右对齐模式，输出更新发生在写 DAC0H 时
}

void DAC0_Output(uint16_t value)
{
    DAC0L = (uint8_t)value;         //送DA值低2位
    DAC0H = (uint8_t)(value >> 8);  //送DA值高8位
}
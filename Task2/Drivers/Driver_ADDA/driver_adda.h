#ifndef DRIVER_ADDA_H
#define DRIVER_ADDA_H

#include "sys.h"

// Timer3
sfr16 TMR3RL = 0x92;  // 定义 TMR3RL 的 sfr16 地址
sfr16 TMR3   = 0x94;  // 定义 TMR3 的 sfr16 地址

#define Disable_Timer3 EIE2 &= 0xFE  // 清除 EIE2 标志位 0，禁止定时器 3 中断
#define Timer3_Reload_Init TMR3RL            // 使用 TMR3RL 重载定时器 3
#define Timer3_Count_Start TMR3    = 0xffff  // 设置 TMR3 计满溢出，立即装载
#define Timer3_Stop        TMR3CN  = 0x02    // 清除 TMR3CN 中断标志位 2，定时器 3 禁止（重置所有状态）
#define Timer3_Start       TMR3CN |= 0x04    // 设定 TMR3CN 中断标志位 2，定时器 3 开启

// ADC
#define SAMPLERATE 25000     // ADC0 采样率 Hz
#define SAR0_CLK            2500000   // ADC0 SAR 转换时钟数值为 2.5MHz
#define Enable_ADC0         EIE2 |= 0x02  // 设定 EIE2 标志位 1，允许 ADC0 转换结束产生的中断请求
#define REF_Ctr_Set         REF0CN  = 0x03  // 关闭温度传感器, 设定芯片上的 VREF
#define ADC0_Gain           ADC0CF &= 0xf8  // ADC0 增益设置，设定 ACD0 的增益为 1
#define ADC0_Clk_Set        ADC0CF  = (SYSCLK / SAR0_CLK) << 3  // 设置 ADC0 SAR 转换时钟周期
#define ADC0_Ctr_Set        ADC0CN  = 0xC4  // ADC0 控制设置，定时器 3 溢出启动跟踪，持续三个 SAR 时钟，然后进行 AD 转换，数据右对齐
#define ADC0_Start          AD0EN   = 1     // 设定 ADC0CN 允许位，ADC0 中断允许
#define ADC0_Stop           AD0EN   = 0     // 清除 ADC0CN 允许位，ADC0 中断禁止
#define ADC0_IntFlag_Clear  AD0INT  = 0     // 清除 ADC0CN 转换结束中断标志位，必须使用软件清零
#define ADC0_Channel        AMX0SL          // ADC0 通道寄存器
#define ADC0_Channel_0      AMX0SL  = 0x00  // ADC0 使用 AIN0 通道
#define ADC0_Channel_1      AMX0SL  = 0x01  // ADC0 使用 AIN1 通道

void ADC0_Init(void);
void ADC0_Enable(uint8_t flag);
uint16_t ADC0_GetValue(void);

// DAC 
#define DAC0_Ctr_Set DAC0CN = 0x80  // 打开 DAC0，右对齐模式，输出更新发生在写 DAC0H 时

void DAC0_Init(void);
void DAC0_Output(uint16_t value);

#endif
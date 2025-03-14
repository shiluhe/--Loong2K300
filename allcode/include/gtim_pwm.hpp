#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include "MAP_ADDR.hpp"

/********************************** 寄存器地址 *************************************/
#define LS_GTIM_BASE_ADDR  0x16119000   // GTIM控制器基地址

/********************************** 寄存器偏移量 *************************************/
#define LS_GTIM_CR1        0x00         // 控制寄存器1
#define LS_GTIM_EGR        0x14         // 事件产生寄存器
#define LS_GTIM_CCMR1      0x18         // 捕获/比较模式寄存器1
#define LS_GTIM_CCMR2      0x1C         // 捕获/比较模式寄存器2
#define LS_GTIM_CCER       0x20         // 捕获/比较使能寄存器
#define LS_GTIM_CNT        0x24         // 计数器
#define LS_GTIM_ARR        0x2C         // 自动重装载寄存器
#define LS_GTIM_CCR1       0x34         // 捕获/比较寄存器1

#define LS_GTIM_CCRx_OFS   0x04   // 捕获/比较寄存器偏移量

/********************************* PWM 模式选择 ************************************/
#define LS_GTIM_Mode_1  0x6    // PWM 模式 1
#define LS_GTIM_Mode_2  0x7    // PWM 模式 2

/********************************* PWM 极性选择 ************************************/
#define LS_GTIM_NORMAL   0x0   // 正相
#define LS_GTIM_INVERSED 0x1   // 反相

class GtimPwm
{
public:
    GtimPwm();
    GtimPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux = 0b11);
    void Enable(void);                      // 使能
    void Disable(void);                     // 失能
    void SetPolarity(uint8_t pol);          // 设置极性
    void SetPeriod(uint32_t cycle);         // 设置周期
    void SetDutyCycle(uint32_t duty_cycle); // 设置占空比
    void SetMode(uint8_t mode);             // 设置 PWM 模式
    ~GtimPwm(void);
public:
    uint8_t  Gpio;        // 引脚号
    uint8_t  ChNum;       // 通道号
    uint8_t  Pol;         // 极性
    uint8_t  Mode;        // 模式
    uint32_t Period;      // 周期
    uint32_t Duty_cycle;  // 占空比
private:
    void *GTIM_CCMR[2]; // 模式寄存器（0--输入；1--输出）
    void *GTIM_CCER;    // 使能寄存器
    void *GTIM_ARR;     // 自动重装载寄存器（周期）
    void *GTIM_CCRx;    // 捕获/比较寄存器1
    void *GTIM_CNT;     // 计数器寄存器
};

void GtimPwmTest();
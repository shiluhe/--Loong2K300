#pragma once

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "HW_gpio.hpp"
#include "MAP_ADDR.hpp"

#define NUM_ENCODER_LINE 512 // 编码器线数

/***********************************寄存器地址**************************************/
#define LS_PWM_BASE_ADDR            0x1611B000  // pwm0基地址
#define LS_PWM_OFFSET               0x10        // pwm控制器偏移量
#define LS_PWM_LOW_BUFFER_OFFSET    0x4         // 低脉冲缓冲寄存器
#define LS_PWM_FULL_BUFFER_OFFSET   0x8         // 脉冲周期缓冲寄存器
#define LS_PWM_CTRL_OFFSET          0xC         // 控制寄存器

/********************************控制寄存器各位域***********************************/
#define LS_PWM_CTRL_EN      BIT(0)  // 计数器使能   （1--计数；0--停止）
#define LS_PWM_CTRL_INTE    BIT(5)  // 中断使能     （1--使能；0--失能）
#define LS_PWM_CTRL_RST     BIT(7)  // 计数器重置   （1--重置；0--正常工作）
#define LS_PWM_CTRL_CAPTE   BIT(8)  // 测量脉冲使能 （1--测量脉冲；0--脉冲输出）

// 编码器模式
class LS_PwmEncoder
{
public:
    LS_PwmEncoder(int channel, int gpio); // 构造函数
    void Init(void);                      // 编码器功能初始化
    float Update(void);                   // 更新
    void ResetCounter(void);              // 重置
    void CloseResCounter(void);           // 关闭重置
    ~LS_PwmEncoder(void);                 // 析构函数
public:
    HWGpio DirGPIO;
private:
    uint32_t PWM_ADDR;
    void *Low_buffer;  // 低脉冲缓冲寄存器
    void *Full_buffer; // 脉冲周期缓冲寄存器
    void *CTRL;        // 控制寄存器
};

void EncoderDemo();
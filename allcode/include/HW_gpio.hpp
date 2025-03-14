#pragma once

#include <iostream>
#include <string>

#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "MAP_ADDR.hpp"

using namespace std;

#define GPIO_Mode_In  1
#define GPIO_Mode_Out 0
/***********************************寄存器地址**************************************/
#define LS_GPIO_BASE_ADDR  0x16104000  // GPIO 基地址

#define LS_GPIO_OEN_OFFSET(n) (0x00 + (n) / 8 * 0x01)  // 方向寄存器：0 --> 输出；1 --> 输入
#define LS_GPIO_O_OFFSET(n)   (0x10 + (n) / 8 * 0x01)  // 输出寄存器
#define LS_GPIO_I_OFFSET(n)   (0x20 + (n) / 8 * 0x01)  // 输入寄存器
/************************************gpio类****************************************/
class HWGpio
{
public:
    HWGpio();                           // 空构造函数
    HWGpio(uint8_t gpio, uint8_t mode); // 设置引脚和模式的构造函数
    void GpioDirection(uint8_t mode);   // 设置输入输出模式
    void SetGpioValue(uint8_t value);   // 设置 GPIO 值
    bool GetGpioValue();                // 获取 GPIO 值
    ~HWGpio();                          // 析构函数
public:
    int Gpio;   // GPIO 引脚号
private:
    void *GPIO_ONE;     // 方向寄存器映射地址
    void *GPIO_O;       // 输出寄存器映射地址
    void *GPIO_I;       // 输入寄存器映射地址
    void *GPIO_REUSE;   // 复用寄存器映射地址
};
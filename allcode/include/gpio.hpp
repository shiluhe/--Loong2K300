#pragma once

#include <iostream>
#include <string>

#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define IN "in"
#define OUT "out"

class SetGPIO
{
public:
    SetGPIO();                                // 构造函数
    SetGPIO(uint8_t gpio, const string &dir); // 带参构造函数
    int GpioExport(uint8_t gpio);             // 导出 gpio
    int GpioUnexport();                       // 取消导出 gpio
    int GpioDirection(const string &dir);     // 设置 gpio 工作模式
    int SetGpioValue(uint8_t val);            // 设置 gpio 电平值
    int GetGpioValue();                       // 获取 gpio 电平值
    ~SetGPIO();                               // 析构函数
public:
    int fd;       // 文件描述符
    uint8_t Gpio; // gpio口
};

void GpioOutputTest1(int gpio);
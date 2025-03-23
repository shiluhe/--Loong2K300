#pragma once

#include <iostream>
#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace std;

#define PWM0 "pwmchip0"  // SPI2_CLK  gpio64   使用通道0
#define PWM1 "pwmchip1"  // SPI2_MISO gpio65   使用通道0
#define PWM2 "pwmchip2"  // SPI2_MOSI gpio66   使用通道0
#define PWM3 "pwmchip3"  // SPI2_CS   gpio67   使用通道0

#define PWM_NORMAL   "normal"   // 正相
#define PWM_INVERSED "inversed" // 反相

// pwm设置类
class SetPWM {
public:
    SetPWM();   // 构造函数
    SetPWM(const string &mold, int cycle, int duty_cycle, const string &polarity);
    //SetPWM(const string &mold, int cycle, const string &polarity);
    int ExportPWM(const string &mold);       // 导出通道
    int UnexportPWM();                       // 关闭通道
    int SetPeriod(int cycle);                // 设置周期
    int SetDutyCycle(int duty_cycle);        // 设置占空比
    int SetPolarity(const string &polarity); // 设置极性
    int Enable();                            // 使能
    int Disable();                           // 失能
    ~SetPWM();  // 析沟函数
public:
    int    fd;          // 文件描述符
    string Mold;        // 设备文件名
    string Channel;     // 通道
    int    Cycle;       // 周期
    int    Duty_Cycle;  // 占空比
    string Polarity;    // 极性
};

void PwmDevTest();
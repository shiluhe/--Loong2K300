#include "main.hpp"

// 蜂鸣器引脚初始化
// HWGpio beep(61, GPIO_Mode_Out);

int main()
{
    // beep.SetGpioValue(0);   // 关闭蜂鸣器

    //GpioOutputTest1(88);    // GPIO输出功能(设备文件)
    // GpioOutputTest2(88);    // GPIO输出功能(硬件)
    // GpioInputTest1();       // GPIO输入功能(设备文件)
    // GpioInputTest2();       // GPIO输入功能(硬件)
    // PwmDevTest();           // PWM 测试(设备文件)
    // PwmHWTest();            // PWM 测试(寄存器)
    //GtimPwmTest();          // Gtim PWM 测试(硬件)
     //EncoderDemo();         // 编码器测试(寄存器)
    //CameraTest();           // 摄像头测试
    // AdcFunTest();           // ADC 功能测试
    // TFTTest();              // TFT屏幕测试
    // GetTimeTest();          // 时间戳打印测试
    // sleepTest();            // sleep()函数测试 -- 以秒为单位延时
    // usleepTest();           // usleep()函数测试 -- 以微秒为单位延时
    // nanosleepTest();        // nanosleep()函数测试 -- 以纳秒为单位延时
    // clock_nanosleepTest();  // clock_nanosleep()函数测试 -- 以纳秒为单位延时
    //BeepOff();
    //MotorTest();            // 电机测试程序
    //ServoTest();            // 舵机测试程序
    // GpioTest();             // 久久派22个GPIO翻转测试
    //Uart_Demo();
    
    Motion1();


    while (1)
    {
    }
    
    return 0;
}
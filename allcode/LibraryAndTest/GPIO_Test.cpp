#include "gpio.hpp"
#include "HW_gpio.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GpioOutputTest1()
 * @功能说明：GPIO 输出功能测试(设备文件)
 * @参数说明：无
 * @函数返回：无
 * @调用方法：GpioOutputTest1();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GpioOutputTest1(int gpio)
{
    SetGPIO Gpio;
    Gpio.GpioExport(83);
    Gpio.GpioDirection(OUT);
    while(1)
    {
        Gpio.SetGpioValue(0);   
        printf("Set gpio value 0\n");
        sleep(1);
        Gpio.SetGpioValue(1);
        printf("Set gpio value 1\n");
        sleep(1);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GpioOutputTest2()
 * @功能说明：GPIO 输出功能测试(寄存器)
 * @参数说明：无
 * @函数返回：无
 * @调用方法：GpioOutputTest2();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GpioOutputTest2(int gpio)
{
    HWGpio Gpio(gpio, GPIO_Mode_Out);
    while(1)
    {
        Gpio.SetGpioValue(0);
        printf("Set HWGpio value 0\n");
        sleep(1);
        Gpio.SetGpioValue(1);
        printf("Set HWGpio value 1\n");
        sleep(1);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GpioTest()
 * @功能说明：久久派22个GPIO翻转测试
 * @参数说明：无
 * @函数返回：无
 * @调用方法：GpioTest();
 * @备注说明：如果使用这个函数之后，板子上的设备文件就无法正常使用，请重启久久派
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
// void GpioTest()
// {
//     #define GPIONUMS 22
//     int gpioNum[GPIONUMS] = {88, 89, 73, 72, 75, 74, 50, 51, 64, 65, 66, 67, 60, 62, 63, 42, 43, 44, 45, 48, 49, 61};
//     HWGpio gpio[GPIONUMS];
//     for (int i = 0; i < GPIONUMS; i++)
//     {
//         gpio[i] = HWGpio(gpioNum[i], GPIO_Mode_Out);
//     }
//     while (1)
//     {
//         for (int i = 0; i < GPIONUMS; i++)
//         {
//             gpio[i].SetGpioValue(1);
//         }
//         printf("Set gpio H\n");
//         sleep(1);
//         for (int i = 0; i < GPIONUMS; i++)
//         {
//             gpio[i].SetGpioValue(0);
//         }
//         printf("Set gpio L\n");
//         sleep(1);
//     }
// }
#include "PWM_Encoder.hpp"
#include "pwm.hpp"
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void EncoderDemo()
 * @功能说明：编码器测试(寄存器，PWM 控制器实现)
 * @参数说明：无
 * @函数返回：无
 * @调用方法：EncoderDemo();
 * @备注说明：可以搭配电机，也可手转查看编码器返回值
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void EncoderDemo()
{
    // 电机PWM
    SetPWM LMotorPWM(PWM1, 100000, 32000, "inversed");
    SetPWM RMotorPWM(PWM2, 100000, 32000, "inversed");
    // 电机方向GPIO
    HWGpio LMotoDir(74, GPIO_Mode_Out);
    HWGpio RMotoDir(75, GPIO_Mode_Out);
    // 初始化编码器计数通道
    LS_PwmEncoder EncodersL(0, 73);
    LS_PwmEncoder EncodersR(3, 72);
    // 初始化电机
    LMotorPWM.Enable();
    RMotorPWM.Enable();
    while(1)
    {
        cout << "left = " << EncodersL.Update() << ", right = " << EncodersR.Update() << endl;
        usleep(1000);
    }
}
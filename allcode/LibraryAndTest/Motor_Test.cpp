#include "HW_gpio.hpp"
#include "pwm.hpp"
#include "PWM_Encoder.hpp"

LS_PwmEncoder EncodersL(0, 73);
LS_PwmEncoder EncodersR(3, 72);
void MotorTest()
{
    /* 电机PWM */
    SetPWM LMotorPWM(PWM1, 100000, 10,"inversed"); 
    SetPWM RMotorPWM(PWM2, 100000, 10,"inversed"); 
    /* 电机方向GPIO */
    HWGpio LMotoDir(74, GPIO_Mode_Out); // CAN3_RX  left motor dir
    HWGpio RMotoDir(75, GPIO_Mode_Out); // CAN3_TX  right motor dir
    // 设置占空比
    LMotorPWM.SetDutyCycle(10000);
    RMotorPWM.SetDutyCycle(10000);
    // 使能
    LMotorPWM.Enable();
    RMotorPWM.Enable();

    // LMotoDir.SetGpioValue(0);
    // RMotoDir.SetGpioValue(0);//需要循环
    while(true)
    {
        // LMotoDir.SetGpioValue(1);
        // RMotoDir.SetGpioValue(1);
        // sleep(1);
        LMotoDir.SetGpioValue(0);
        RMotoDir.SetGpioValue(0);
        cout << "left = " << -EncodersL.Update() << ", right = " << EncodersR.Update() << endl;

        sleep(1);
    }
}

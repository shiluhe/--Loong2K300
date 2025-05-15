#include "Motor.hpp"


// /* 电机PWM */
// SetPWM LMotorPWM(PWM1, 100000, 10, "inversed"); 
// SetPWM RMotorPWM(PWM2, 100000, 10, "inversed"); 
// /* 电机方向GPIO */
// HWGpio LMotoDir(74, GPIO_Mode_Out); // CAN3_RX  left motor dir
// HWGpio RMotoDir(75, GPIO_Mode_Out); // CAN3_TX  right motor dir
// void MotorRun(){
//     // 设置占空比
//     LMotorPWM.SetDutyCycle(8000);
//     RMotorPWM.SetDutyCycle(8000);
//     LMotoDir.SetGpioValue(0);
//     RMotoDir.SetGpioValue(0);
//     // 使能PWM（首次调用时启用）
//     static bool is_enabled = false;
//     if (!is_enabled) {
//         LMotorPWM.Enable();
//         RMotorPWM.Enable();
//         is_enabled = true;
//     }
    // while(true)
    // {
        // LMotoDir.SetGpioValue(0);
        // RMotoDir.SetGpioValue(0);
        // sleep(2);
        // LMotoDir.SetGpioValue(0);
        // RMotoDir.SetGpioValue(0);
        // sleep(2);
    // }
// }

MotorController::MotorController(double kp_l, double ki_l, double kd_l, double kp_r, double ki_r, double kd_r)
    : LMotorPWM(PWM1, 100000, 10,"inversed"),  // 初始化列表
      RMotorPWM(PWM2, 100000, 10,"inversed"),
      LMotoDir(74, GPIO_Mode_Out),
      RMotoDir(75, GPIO_Mode_Out),
      EncoderL(0, 73),
      EncoderR(3, 72),
      pidL(kp_l, ki_l, kd_l, PIDMode::Inc),
      pidR(kp_r, ki_r, kd_r, PIDMode::Inc)
{
    LMotorPWM.SetDutyCycle(0);
    RMotorPWM.SetDutyCycle(0);//占空比10000/100000
    // 使能PWM（首次调用时启用）
    static bool is_enabled = false;
    if (!is_enabled) {
        LMotorPWM.Enable();
        RMotorPWM.Enable();
        is_enabled = true;
    }
}

MotorController::~MotorController(){
    LMotorPWM.SetDutyCycle(0);
    RMotorPWM.SetDutyCycle(0);
}


void MotorController::BothMotorRun(double left_target, double right_target){
        // 控制左电机
        float currentSpeedL = -EncoderL.Update();
        float errorL = left_target - currentSpeedL;
        if(abs(errorL) < 0.1) errorL = 0;
        double pidOutputL = pidL.Tick(errorL);
        
        // 控制右电机
        float currentSpeedR = EncoderR.Update();
        float errorR = right_target - currentSpeedR;
        if(abs(errorR) < 0.1) errorR = 0;
        double pidOutputR = pidR.Tick(errorR);

        LMotoDir.SetGpioValue(0);
        RMotoDir.SetGpioValue(0);

        LMotorPWM.SetDutyCycle(static_cast<uint32_t>(abs(pidOutputL) * 1000));
        RMotorPWM.SetDutyCycle(static_cast<uint32_t>(abs(pidOutputR) * 1000));

        //调试输出
        // printf("L：Current:%.2f Error:%.2f Output:%.2f\n", currentSpeedL, errorL, pidOutputL);
        // printf("R：Current:%.2f Error:%.2f Output:%.2f\n", currentSpeedR, errorR, pidOutputR);
        // printf("================================================================\n");
        //VOFA+打印
        Vofa_Uart_speed(currentSpeedL, currentSpeedR, left_target, pidOutputL);


}

void BeepOff(){
    HWGpio Beep(61, GPIO_Mode_Out);
    Beep.SetGpioValue(0);
}


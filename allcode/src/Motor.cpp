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
//     // while(true)
//     // {
//         // LMotoDir.SetGpioValue(0);
//         // RMotoDir.SetGpioValue(0);
//         // sleep(2);
//         // LMotoDir.SetGpioValue(0);
//         // RMotoDir.SetGpioValue(0);
//         // sleep(2);
//     // }
// }

MotorController::MotorController()
    : LMotorPWM(PWM1, 100000, 10, "inversed"),  // 初始化列表
      RMotorPWM(PWM2, 100000, 10, "inversed"),
      LMotoDir(74, GPIO_Mode_Out),
      RMotoDir(75, GPIO_Mode_Out) {

    LMotorPWM.SetDutyCycle(10000);
    RMotorPWM.SetDutyCycle(10000);//占空比10000/100000
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

void MotorController::MotorRun(){
    LMotoDir.SetGpioValue(0);
    RMotoDir.SetGpioValue(0);
}

void BeepOff(){
    HWGpio Beep(61, GPIO_Mode_Out);
    Beep.SetGpioValue(0);
}


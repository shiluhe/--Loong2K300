#include "Servo.hpp"

// GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1); // TIM2_CH2 GPIO88
// PID pid(3,0.01,0.5);

// void ServoRun(double error){
//     double output = pid.calculate(error);
//     //GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1); // TIM2_CH2 GPIO88
//     //GtimPwm ServoPWM2(89, 3, LS_GTIM_INVERSED, 2000000 - 1); // TIM2_CH3 GPIO89
//     ServoPWM1.Enable();
//     //ServoPWM2.Enable();


// //舵机角度约束
//     double anglecontrol = 0;
//     anglecontrol = 170000 - output * 30;
//     if(anglecontrol > 180000){
//         anglecontrol = 180000;
//     }
//     else if (anglecontrol < 160000){
//         anglecontrol = 160000;
//     }
//     //ServoPWM1.SetDutyCycle(160000);
//     ServoPWM1.SetDutyCycle(anglecontrol);
//     printf("anglecontrol = %.2f\n" , anglecontrol);
//     //printf("output = %.2f\n" , output);
// }


ServoController::ServoController(int pin, double kp, double ki, double kd, double minAngle, double maxAngle, double centerAngle)
      : pwm(pin, 2, LS_GTIM_INVERSED, 2000000 - 1),
        pid(kp, ki, kd),
        minAngle(minAngle),
        maxAngle(maxAngle),
        centerAngle(centerAngle){
        pwm.Enable();  // 初始化时启用 PWM
        }
ServoController::~ServoController(){
    pwm.Disable(); //禁用pwm
}
/*
160000：右边走
170000:中间
180000：左边走
*/
void ServoController::Run(double error) {
    double output = pid.calculate(error);
    double angleControl = centerAngle - output * 30;  // 计算目标角度

    // 限制角度范围
    if (angleControl > maxAngle) {
        angleControl = maxAngle;
    } else if (angleControl < minAngle) {
        angleControl = minAngle;
    }

    // 设置 PWM 占空比
    pwm.SetDutyCycle(angleControl);

    // 打印调试信息
    printf("angleControl = %.2f\n", angleControl);
}
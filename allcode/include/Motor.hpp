#include "HW_gpio.hpp"
#include "pwm.hpp"
#include "PWM_Encoder.hpp"
#include "pid.hpp"
#include "Uart.hpp"
enum class MotorMode{
    Left = 1,
    Right =2,
};

class MotorController{
private:
    SetPWM LMotorPWM;
    SetPWM RMotorPWM;
    HWGpio LMotoDir;
    HWGpio RMotoDir;
    LS_PwmEncoder EncoderL;
    LS_PwmEncoder EncoderR;
    PID pidL;
    PID pidR;
public:
    MotorController(double kp_l, double ki_l, double kd_l, double kp_r, double ki_r, double kd_r);
    ~MotorController();
    void BothMotorRun(double left_target, double right_target);

};

extern MotorController Motor;

void BeepOff();
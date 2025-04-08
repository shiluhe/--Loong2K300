#include "HW_gpio.hpp"
#include "pwm.hpp"

class MotorController{
private:
    SetPWM LMotorPWM;
    SetPWM RMotorPWM;
    HWGpio LMotoDir;
    HWGpio RMotoDir;
public:
    MotorController();
    ~MotorController();
    void MotorRun();

};

extern MotorController Motor;

void BeepOff();
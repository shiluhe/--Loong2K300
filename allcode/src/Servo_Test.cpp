#include "gtim_pwm.hpp"

class PID{
public:
    PID(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd), error(0), pre_error(0), integral(0) {}
    double calculate(double error){
        this->error = error;
        this->derivative = this->error - this->pre_error;
        this->integral += this->error;
        this->pre_error = this->error;
        return this->kp * this->error + this->ki * this->integral + this->kd * this->derivative;
    }
private:
double kp;
double ki;
double kd;
double error;
double pre_error;
double integral;
double derivative;

};

void ServoTest()
{
    /* 舵机PWM */
    GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1, 150000); // TIM2_CH2 GPIO88
    GtimPwm ServoPWM2(89, 3, LS_GTIM_INVERSED, 2000000 - 1, 150000); // TIM2_CH3 GPIO89
    ServoPWM1.Enable();
    ServoPWM2.Enable();
    while (1)
    {
        ServoPWM1.SetDutyCycle(100000);
        ServoPWM2.SetDutyCycle(100000);
        printf("PWM Set %d\n", 100000);
        sleep(1);
        ServoPWM1.SetDutyCycle(150000);
        ServoPWM2.SetDutyCycle(150000);
        printf("PWM Set %d\n", 150000);
        sleep(1);
        ServoPWM1.SetDutyCycle(200000);
        ServoPWM2.SetDutyCycle(200000);
        printf("PWM Set %d\n", 200000);
        sleep(1);
    }
}

void ServoTest1(){
    GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1); // TIM2_CH2 GPIO88
    ServoPWM1.Enable();
    ServoPWM1.SetDutyCycle(170000);
}

void ServoRun(double error){
    PID pid(3,0.01,0.5);
    double output = pid.calculate(error);
    GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1); // TIM2_CH2 GPIO88
    //GtimPwm ServoPWM2(89, 3, LS_GTIM_INVERSED, 2000000 - 1); // TIM2_CH3 GPIO89
    ServoPWM1.Enable();
    //ServoPWM2.Enable();

/*
160000：右边走
170000:中间
180000：左边走
*/
//舵机角度约束
    double anglecontrol = 0;
    anglecontrol = 170000 - output * 50;
    if(anglecontrol > 180000){
        anglecontrol = 180000;
    }
    else if (anglecontrol < 160000){
        anglecontrol = 160000;
    }
    //ServoPWM1.SetDutyCycle(160000);
    ServoPWM1.SetDutyCycle(anglecontrol);
    printf("anglecontrol = %.2f\n" , anglecontrol);
    printf("output = %.2f\n" , output);

}
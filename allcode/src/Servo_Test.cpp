#include "gtim_pwm.hpp"


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
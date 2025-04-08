#include "gtim_pwm.hpp"
#include "pid.hpp"



/**
* @brief 构造函数，初始化舵机控制
* @param pin PWM 引脚（例如 88）
* @param kp PID 的 P 参数
* @param ki PID 的 I 参数
* @param kd PID 的 D 参数
* @param minAngle 舵机最小角度（默认 160000）
* @param maxAngle 舵机最大角度（默认 180000）
* @param centerAngle 舵机中间角度（默认 170000）
*/
class ServoController {
private:
    GtimPwm pwm;  // PWM 控制对象
    PID pid;      // PID 控制器
    double minAngle;  // 最小角度（例如 160000）
    double maxAngle;  // 最大角度（例如 180000）
    double centerAngle;  // 中间角度（例如 170000）
public:
    ServoController(int pin, double kp, double ki, double kd,
                    double minAngle = 160000.0,
                    double maxAngle = 180000.0,
                    double centerAngle = 170000.0);
    ~ServoController();
    void Run(double error);
};

extern ServoController Servo;



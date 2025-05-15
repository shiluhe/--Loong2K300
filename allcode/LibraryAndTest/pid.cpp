#include "pid.hpp"

PID::PID(double kp, double ki, double kd, PIDMode mode)
    : kp(kp),
      ki(ki),
      kd(kd),
      error(0),
      pre_error(0),
      prepre_error(0),
      integral(0),
      mode(mode) {}

double PID::Tick(double error) {  // 添加了PID::作用域限定符
    switch (mode) {
    case PIDMode::Inc:
        return IncTick(error);
    case PIDMode::Pos:
        return PosTick(error);
    default:
        return PosTick(error);
    }
}

double PID::PosTick(double error) {
    error = error / max_error;
    this->error = error;
    this->derivative = this->error - this->pre_error;
    this->integral += this->error;  // 统一使用integral

    if (this->integral > max_integral) this->integral = max_integral;
    if (this->integral < -max_integral) this->integral = -max_integral;

    double output = this->kp * this->error + this->ki * this->integral + this->kd * this->derivative;

    if (output > max_outputPos) output = max_outputPos;
    if (output < min_outputPos) output = min_outputPos;

    this->pre_error = this->error;
    return output;
}

double PID::IncTick(double error) {
    this->prepre_error = pre_error;
    this->pre_error = this->error;
    this->error = error;

    double output =
        this->kp * (this->error - this->pre_error)
        + this->ki * this->error
        + this->kd * (this->error - 2 * this->pre_error + this->prepre_error);

    this->output += output;
    if (this->output > max_outputInc) this->output = max_outputInc;
    if (this->output < min_outputInc) this->output = min_outputInc;
    return this->output;
}
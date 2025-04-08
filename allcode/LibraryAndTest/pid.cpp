#include "pid.hpp"

PID::PID(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd), error(0), pre_error(0), integral(0) {}
double PID::calculate(double error){
        this->error = error;
        this->derivative = this->error - this->pre_error;
        this->integral += this->error;
        this->pre_error = this->error;
        return this->kp * this->error + this->ki * this->integral + this->kd * this->derivative;
    }

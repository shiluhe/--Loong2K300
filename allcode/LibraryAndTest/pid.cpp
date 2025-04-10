#include "pid.hpp"

PID::PID(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd), error(0), pre_error(0), integral(0) {}
double PID::calculate(double error){
        error = error / 80.0;
        this->error = error;
        this->derivative = this->error - this->pre_error;
        this->integral += this->error;

        if(this->integral > max_integral) this->integral = max_integral;
        if(this->integral < -max_integral) this->integral = -max_integral;

        double output = this->kp * this->error + this->ki * this->integral + this->kd * this->derivative;

        if (output > max_output) output = max_output;
        if (output < min_output) output = min_output;

        this->pre_error = this->error;
        return output;
    }

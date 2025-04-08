#ifndef PID_HPP
#define PID_HPP

class PID{
private:
    double kp;
    double ki;
    double kd;
    double error;
    double pre_error;
    double integral;
    double derivative;
public:
    PID(double kp, double ki, double kd);
    double calculate(double error);

};
#endif
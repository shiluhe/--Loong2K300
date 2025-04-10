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
    double max_integral=1;
    double max_output = 1.0;
    double min_output = -1.0; 
public:
    PID(double kp, double ki, double kd);
    double calculate(double error);

};
#endif
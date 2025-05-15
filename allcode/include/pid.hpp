#ifndef PID_HPP
#define PID_HPP

enum class PIDMode {
    Inc = 1,
    Pos = 2,
};

class PID {
private:
    double kp;
    double ki;
    double kd;
    double error;
    double pre_error;
    double prepre_error;
    double integral;  // 统一使用integral
    double derivative;
    double max_integral = 0.5;
    double max_outputPos = 1.0;
    double min_outputPos = -1.0;
    double max_outputInc = 10.0;
    double min_outputInc = -10.0;
    double max_error = 80.0;
    double output;
    PIDMode mode;

public:
    PID(double kp, double ki, double kd, PIDMode mode = PIDMode::Pos);
    double Tick(double error);
    double IncTick(double error);
    double PosTick(double error);
};

#endif
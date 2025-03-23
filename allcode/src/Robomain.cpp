#include "main.hpp"
#include "image.hpp"
#include <thread>
#include "pwm.hpp"

void MotorRunthread(){
    while (true) {
        MotorRun();
        this_thread::sleep_for(chrono::milliseconds(20)); // 模拟电机运行
    }
}

double image_processthread() {
    double error = image_process(); // 调用 image_process 并获取返回值
    //this_thread::sleep_for(chrono::milliseconds(10)); // 模拟图像处理
    return error; // 返回 image_process 的返回值

}

void ServoRunthread(double error) {
    ServoRun(error);
    //ServoTest1();
    //cout << "Servo running with error: " << error << endl;
    //this_thread::sleep_for(chrono::milliseconds(100)); // 模拟舵机控制
}

void Motion(){
    double error = 0;
    // 创建一个线程运行 MotorRun
    thread motorThread(MotorRunthread);
    while(1)
    {
    error = image_processthread();
    ServoRunthread(error);
    //ServoRunthread(30);
    }
    
    //等待电机线程结束（通常不会执行到这里）
    motorThread.join();

}
#pragma once

#include <iostream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>//time_func

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>    // 共享内存
#include <sys/sem.h>    // 信号量集

#include <atomic>    // 添加atomic头文件
#include <mutex>     // 添加mutex头文件
#include <sched.h>
#include <thread>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "Uart.hpp"
#include "gpio.hpp"
#include "pwm.hpp"
#include "gtim_pwm.hpp"
#include "PWM_Encoder.hpp"

using namespace cv;
using namespace std;

void Motion1();
void MotorTest();
void BeepOff();
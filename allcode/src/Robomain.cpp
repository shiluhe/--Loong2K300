#include "main.hpp"
#include "image.hpp"
#include "Servo.hpp"
#include "Motor.hpp"
#include "TFT.hpp"

mutex frame_mutex;
mutex error_mutex;
double current_error = 0;
Mat current_frame;
// 设置线程优先级和调度策略
// void SetThreadPriority(pthread_t thread_id, int policy, int priority) {
//     sched_param sch_params;
//     sch_params.sched_priority = priority;
//     if (pthread_setschedparam(thread_id, policy, &sch_params)) {
//         cerr << "Failed to set thread priority (需要root权限或CAP_SYS_NICE能力)" << endl;
//     }
// }

// // 电机控制线程（低优先级）
// void MotorRunThread() {
//     pthread_t this_thread = pthread_self();
//     SetThreadPriority(this_thread, SCHED_OTHER, 0);  // 默认策略，优先级最低

//     while (true) {
//         MotorRun();
//     }
// }

// // 图像处理线程（中低优先级）
// void ImageProcessThread() {
//     pthread_t this_thread = pthread_self();
//     SetThreadPriority(this_thread, SCHED_RR, 50);  // 中等优先级

//     Mat frame;
//     {
//     lock_guard<mutex> lock(frame_mutex);
//     if (!current_frame.empty()) {
//         frame = current_frame.clone();}
//     }
//     if (!frame.empty()) {
//         double error = image_process(frame);  // 图像处理
//         {
//             lock_guard<mutex> lock(error_mutex);
//             current_error = error;
//         }
//     }
// }

// // 舵机控制线程（高优先级）
// void ServoControlThread() {
//     pthread_t this_thread = pthread_self();
//     SetThreadPriority(this_thread, SCHED_FIFO, 80);  // 高优先级，实时调度

//     while (true) {
//         double error = 0;
//         {
//             lock_guard<mutex> lock(error_mutex);
//             error = current_error;
//         }
//         printf("Current error: %.3f\n", error);  // 保留3位小数
//         //ServoRun(0); 
//         ServoRun(error);  // 舵机控制
//     }
//     //ServoRun(0);  // 舵机控制
// }

// // 摄像头采集线程（最高优先级）
// void CameraCaptureThread() {
//     pthread_t this_thread = pthread_self();
//     SetThreadPriority(this_thread, SCHED_FIFO, 99);  // 最高优先级

//     while (true) {
//         Mat frame = capture_frame();  // 假设的摄像头采集函数
//         if(!frame.empty()){
//             lock_guard<mutex> lock(frame_mutex);  // 加锁
//             current_frame = frame;
//         }
//         //process_frame(frame);         // 传递给图像处理（需同步）
//     }
// }

// void Motion() {
//     thread motor_thread(MotorRunThread);
//     thread image_thread(ImageProcessThread);
//     thread servo_thread(ServoControlThread);
//     thread camera_thread(CameraCaptureThread);

//     // 主线程监控（可选）
//     while (true) {
//         this_thread::sleep_for(chrono::seconds(1));
//     }

//     // 安全退出（实际需添加信号处理）
//     motor_thread.join();
//     image_thread.join();
//     servo_thread.join();
//     camera_thread.join();

// }

void RoboInit(){
    Camera_Init();
    TFTSPI_Init(0);//占用0.2s左右
}
void Motion1(){
    BeepOff();
    RoboInit();
    MotorController Motor{0.3, 0.1, 0.03, 0.3, 0.1, 0.03};
    ServoController Servo(88, 2, 0, 0.01);
    sleep(1);
    const double controlPeriod = 0.001; // 1ms控制周期

    while(1){
        
        auto start = chrono::high_resolution_clock::now();  // 记录起始时间
        Motor.BothMotorRun(10.0, 10.0);
        
        // Mat frame = capture_frame();  // 假设的摄像头采集函数
        // double error = image_process(frame);  // 图像处理
        // Servo.Run(error);  // 舵机控制, error摄像头两线偏差
        //Servo.Run(80);
       // 精确延时保证固定周期
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        double sleepTime = controlPeriod - elapsed.count();
        if(sleepTime > 0) {
        usleep(static_cast<useconds_t>(sleepTime * 1e6));}
}
}
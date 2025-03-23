#include "main.hpp"

void CameraTest()
{
    // 创建一个摄像头
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cerr << "Error open video stream" << endl;
        return;
    }
    // 设置视频流编码器
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // 设置摄像头图像宽高和帧率
    cap.set(CAP_PROP_FRAME_WIDTH, 160);
    cap.set(CAP_PROP_FRAME_HEIGHT, 120);
    cap.set(CAP_PROP_FPS, 60);
    // 获取摄像头图像宽高和帧率
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double frame_fps = cap.get(CAP_PROP_FPS);
    printf("frame:%d*%d, fps:%3f", frame_width, frame_height, frame_fps);
    sleep(1);
    // 获取视频流
    Mat frame;
    while (1)
    {
        // 读取摄像头一帧图像
        cap.read(frame);    // cap >> frame;
        if (frame.empty())
        {
            cerr << "Error read frame" << endl;
            break;
        }
        // 转化为灰度
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        // 二值化处理
        Mat binary;
        threshold(gray, binary, 127, 255, THRESH_BINARY | THRESH_OTSU);
        // 获取图像数据
        for (int i = 0; i < 160; i+=2)
        {
            for (int j = 0; j < 120; j+=2)
            {
                printf("%4d", binary.at<unsigned char>(i, j));
            }
            printf("\n");
        }
        printf("\n\n");
    }
} 


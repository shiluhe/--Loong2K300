#pragma once

#include <linux/spi/spidev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

class HW_SPI
{
public:
    HW_SPI(); // 构造函数
    HW_SPI(const char *file, 
           uint8_t mode = SPI_MODE_0,
           uint8_t bits = 8,
           uint32_t speed = 50000000);              // 带参构造函数
    void SetMode(uint8_t mode);                     // 设置 SPI 模式
    void SetBits(uint8_t bits);                     // 设置收发数据位数
    void SetSpeed(uint32_t speed);                  // 设置传输速度
    void SPI_Write(uint8_t tx[], uint8_t tx_Num);   // 发送数据
    ~HW_SPI(); // 析构函数
public:
    int spi_fd; // 存放spi设备文件的文件描述符
private:
    uint8_t  Mode;  // 模式
    uint8_t  Bits;  // 字节数
    uint32_t Speed; // 速度
    struct spi_ioc_transfer Tr;
};
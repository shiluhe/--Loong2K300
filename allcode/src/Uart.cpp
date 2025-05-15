#include "Uart.hpp"

//初始化UART1
LS_UART uart(UART1, B115200, LS_UART_STOP1, LS_UART_DATA8, LS_UART_NONE);
void Vofa_Uart_speed(float currentSpeedL, float currentSpeedR, float left_target, float right_target) {
    char tx_buffer[64]; // 足够存储格式化后的字符串
    
    //格式化字符串到缓冲区
    int len = snprintf(tx_buffer, sizeof(tx_buffer), 
                     "%.2f,%.2f,%.2f,%.2f\n", 
                     currentSpeedL, currentSpeedR, 
                     left_target, right_target);
    
    //检查格式化是否成功（len > 0）
    if (len <= 0) {
        printf("Format error!\n");
        return;
    }

    //通过串口发送数据
    int ret = uart.WriteData(tx_buffer, len);
    
    //检查发送结果
    if (ret < 0) {
        printf("UART write error!\n");
    }
}
#include "Uart.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void Uart_Demo()
 * @功能说明：串口功能测试
 * @参数说明：无
 * @函数返回：无
 * @调用方法：Uart_Demo();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Uart_Demo()
{
    LS_UART uart(UART1, B115200, LS_UART_STOP1, LS_UART_DATA8, LS_UART_NONE);
    
    while(1) {
        char buf[256];
        int ret = uart.ReadData(buf, sizeof(buf));
        if (ret < 0) {
            printf("Reading error!\n");
            return;
        }
        buf[ret] = '\0';
        printf("Receive %d bytes : %s\n", ret, buf);
        ret = uart.WriteData(buf, ret);
        if (ret < 0) {
            printf("Writeing error!\n");
            return;
        }
        sleep(1);
    }
    return;
}



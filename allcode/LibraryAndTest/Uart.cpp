#include "Uart.hpp"  

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：LS_UART::LS_UART()
 * @功能说明：串口类的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：LS_UART uart();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_UART::LS_UART()
{

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：LS_UART::LS_UART(const string &uart, speed_t baudrate, uint8_t stop, uint8_t data, uint8_t check)
                : BaudRate(baudrate), Stop(stop), Data(data), Check(check)
 * @功能说明：串口类的带参构造函数
 * @参数说明：uart     : 所使用的串口设备文件
 * @参数说明：baudrate : 波特率设置
 *      可选择波特率: B0       B50      B75      B110     B134     B150
 *                   B200     B300     B600     B1200    B1800    B2400
 *                   B4800    B9600    B19200   B38400   B57600   B115200
 *                   B230400  B460800  B500000  B576000  B921600
 * @参数说明：stop     : 停止位设置
 *      可选择停止位: LS_UART_STOP1 --> 1位停止位
 *                   LS_UART_STOP2 --> 2位停止位
 * @参数说明：data     : 数据位设置
 *      可选择数据位: LS_UART_DATA5 --> 5位数据位
 *                   LS_UART_DATA6 --> 6位数据位
 *                   LS_UART_DATA7 --> 7位数据位
 *                   LS_UART_DATA8 --> 8位数据位
 * @参数说明：check    : 校验位设置
 *      可选择校验位: LS_UART_NONE --> 无校验
 *                   LS_UART_ODD  --> 偶校验
 *                   LS_UART_EVEN --> 奇校验
 * @函数返回：无
 * @调用方法：LS_UART uart(LS_UART1, B19200, LS_UART_STOP1, LS_UART_DATA8, LS_UART_NONE);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_UART::LS_UART(const string &uart, speed_t baudrate, uint8_t stop, uint8_t data, uint8_t check)
 : BaudRate(baudrate), Stop(stop), Data(data), Check(check)
{
    // 打开串口设备文件
    this->uart_fd = open(uart.c_str(), O_RDWR);  
    if (this->uart_fd == -1) {  
        printf("Unable to open dev!\n");  
        return;
    }
    // 串口配置初始化
    memset(&this->ts, 0, sizeof(this->ts));
    if (tcgetattr(this->uart_fd, &this->ts) != 0)
    {
        perror("Error tcgetattr");
        close(this->uart_fd);
        return;
    }
    this->ts.c_cflag &= ~CRTSCTS;                       // 无硬件流控制
    this->ts.c_cflag |= CREAD | CLOCAL;                 // 打开接收器，忽略modem控制线
    this->ts.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);// 原始输入  
    this->ts.c_iflag &= ~(IXON | IXOFF | IXANY);		// 禁用软件流控  
    this->ts.c_oflag &= ~OPOST; 			        	// 原始输出  
    this->ts.c_cc[VMIN] = 0; 							// 读取时不等待  
    this->ts.c_cc[VTIME] = 5; 			    		    // 0.5秒超时
    // 设置波特率
    this->SetBaudrate(this->BaudRate);
    // 设置停止位
    this->SetStopBit(this->Stop);
    // 设置数据位
    this->SetDataBit(this->Data);
    // 设置校验位
    this->SetChecBit(this->Check);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void LS_UART::SetBaudrate(speed_t baudrate)
 * @功能说明：设置串口波特率
 * @参数说明：baudrate : 波特率设置
 * @函数返回：无
 * @调用方法：uart.SetBaudrate(B19200);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetBaudrate(speed_t baudrate)
{
    this->BaudRate = baudrate;
    cfsetispeed(&this->ts, baudrate);   // 设置输入波特率
    cfsetospeed(&this->ts, baudrate);   // 设置输出波特率
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Baudrate");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void LS_UART::SetStopBit(uint8_t stop)
 * @功能说明：设置串口停止位
 * @参数说明：stop : 停止位设置
 * @函数返回：无
 * @调用方法：uart.SetStopBit(LS_UART_STOP1);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetStopBit(uint8_t stop)
{
    this->Stop = stop;
    switch(this->Stop)
    {
        case LS_UART_STOP1:
            this->ts.c_cflag &= ~CSTOPB;break;  // 停止位为1
        case LS_UART_STOP2:
            this->ts.c_cflag |= CSTOPB;break;   // 停止位为2
        default:
            printf("Stop bit flag setting error");
            break;
    }
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Stop Bit");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void LS_UART::SetDataBit(uint8_t data)
 * @功能说明：设置串口数据位
 * @参数说明：data : 停止位设置
 * @函数返回：无
 * @调用方法：uart.SetDataBit(LS_UART_DATA8);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetDataBit(uint8_t data)
{
    this->Data = data;
    this->ts.c_cflag &= ~CSIZE;    // 清除数据位大小设置
    switch(this->Data)
    {
        case LS_UART_DATA5: // 数据位为5
            this->ts.c_cflag |= CS5;break;
        case LS_UART_DATA6: // 数据位为6
            this->ts.c_cflag |= CS6;break;
        case LS_UART_DATA7: // 数据位为7
            this->ts.c_cflag |= CS7;break;
        case LS_UART_DATA8: // 数据位为8
            this->ts.c_cflag |= CS8;break;
        default:
            printf("Data bit flag setting error");
            break;
    }
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Stop Bit");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void LS_UART::SetChecBit(uint8_t check)
 * @功能说明：设置串口校验位
 * @参数说明：check : 停止位设置
 * @函数返回：无
 * @调用方法：uart.SetChecBit(LS_UART_NONE);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetChecBit(uint8_t check)
{
    this->Check = check;
    this->ts.c_cflag &= ~PARENB;    // 清除校验位
    switch (this->Check)
    {
        case LS_UART_NONE:  // 无校验
            break;
        case LS_UART_ODD:   // 偶校验
            this->ts.c_cflag |= PARENB;
            this->ts.c_cflag &= PARODD;
            break;
        case LS_UART_EVEN:  // 奇校验
            this->ts.c_cflag |= PARENB;
            this->ts.c_cflag |= PARODD;
            break;
        default:
            printf("Check bit flag setting error");
            break;
    }
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Check Bit");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：ssize_t LS_UART::WriteData(const char *buf, ssize_t len)
 * @功能说明：设置串口发送函数
 * @参数说明：buf : 发送缓冲区
 * @参数说明：len : 发送数据长度
 * @函数返回：无
 * @调用方法：uart.WriteData(buf, sizeof(buf));
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t LS_UART::WriteData(const char *buf, ssize_t len)
{
    return write(this->uart_fd, buf, len);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：ssize_t LS_UART::ReadData(char *buf, ssize_t len)
 * @功能说明：设置串口接收函数
 * @参数说明：buf : 接收缓冲区
 * @参数说明：len : 接收数据长度
 * @函数返回：无
 * @调用方法：uart.ReadData(buf, sizeof(buf));
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t LS_UART::ReadData(char *buf, ssize_t len)
{
    return read(this->uart_fd, buf, len);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：LS_UART::~LS_UART()
 * @功能说明：串口类的析构构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_UART::~LS_UART()
{
    close(this->uart_fd);
}
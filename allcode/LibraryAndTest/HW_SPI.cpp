#include "HW_SPI.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HW_SPI::HW_SPI()
 * @功能说明：硬件 SPI 的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：HW_SPI MySPI;
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HW_SPI::HW_SPI()
{

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HW_SPI::HW_SPI(const char *file, uint8_t mode, uint8_t bits, uint32_t speed) :
            Mode(mode), Bits(bits), Speed(speed)
 * @功能说明：硬件 SPI 的有参构造函数
 * @参数说明：file   :  SPI 设备文件及其路径
 * @参数说明：mode   :  SPI 模式，默认模式为SPI_MODE_0
 * @参数说明：bits   :  数据传输位数，默认为8位
 * @参数说明：speed  :  数据传输速度，默认速度为50000000
 * @函数返回：无
 * @调用方法：HW_SPI MySPI("/dev/spidev1.0", SPI_MODE_0, 8, 50000000);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HW_SPI::HW_SPI(const char *file, uint8_t mode, uint8_t bits, uint32_t speed) :
            Mode(mode), Bits(bits), Speed(speed)
{
    /* 打开 SPI 设备文件 */
    this->spi_fd = open(file, O_RDWR);
    if (this->spi_fd < 0)
    {
        perror("open spi file error");
        return;
    }
    /* 设置 SPI 模式 */
    this->SetMode(this->Mode);
    /* 设置 SPI 每次发送或接收字节数 */
    this->SetBits(this->Bits);
    /* 设置 SPI 数据传输速度 */
    this->SetSpeed(this->Speed);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HW_SPI::SetMode(uint8_t mode)
 * @功能说明：设置硬件 SPI 的模式
 * @参数说明：mode  :  硬件 SPI 模式
 * @函数返回：无
 * @调用方法：MySPI.SetMode(SPI_MODE_0);
 * @备注说明：SPI 的传输模式有四种
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SetMode(uint8_t mode)
{
    this->Mode = mode;
    ioctl(this->spi_fd, SPI_IOC_WR_MODE, &this->Mode);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HW_SPI::SetBits(uint8_t bits)
 * @功能说明：设置硬件 SPI 的位数
 * @参数说明：mode  :  SPI 传输数据的位数
 * @函数返回：无
 * @调用方法：MySPI.SetBits(8);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SetBits(uint8_t bits)
{
    this->Bits = bits;
    ioctl(this->spi_fd, SPI_IOC_WR_BITS_PER_WORD, &this->Bits);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HW_SPI::SetSpeed(uint32_t speed)
 * @功能说明：设置硬件 SPI 的传输速度
 * @参数说明：mode  :  数据传输速度
 * @函数返回：无
 * @调用方法：MySPI.SetSpeed(50000000);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SetSpeed(uint32_t speed)
{
    this->Speed = speed;
    ioctl(this->spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &this->Speed);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HW_SPI::SPI_Write(uint8_t tx[], uint8_t tx_Num)
 * @功能说明：硬件 SPI 的数据发送函数
 * @参数说明：tx      :  发送数据的数组
 * @参数说明：tx_Num  :  发送数据的长度
 * @函数返回：无
 * @调用方法：MySPI.SPI_Write(tx, sizeof(tx));
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SPI_Write(uint8_t tx[], uint8_t tx_Num)
{
    this->Tr = {
        .tx_buf = (unsigned long)tx,    // 设置发送缓冲区
        .rx_buf = 0,                    // 不接收数据，所以设置为0
        .len = tx_Num,                  // 设置发送数据的长度
    };
    /* 执行 SPI 传输操作 */
    int ret = ioctl(this->spi_fd, SPI_IOC_MESSAGE(1), &this->Tr);
    if (ret < 0)
    {
        perror("can't send spi message");
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HW_SPI::~HW_SPI()
 * @功能说明：硬件 SPI 的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HW_SPI::~HW_SPI()
{
    close(this->spi_fd);
}
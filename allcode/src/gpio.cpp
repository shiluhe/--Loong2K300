#include "gpio.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：SetGPIO::SetGPIO()
 * @功能说明：GPIO 类的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：SetGPIO MyGpio;
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
SetGPIO::SetGPIO()
{
    this->fd = -1;
    this->Gpio = -1;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：SetGPIO::SetGPIO(uint8_t gpio, const string &dir)
 * @功能说明：GPIO 类的带参构造函数，配置 GPIO 并设置模式
 * @参数说明：gpio  :  对应使用的引脚号
 * @参数说明：dir   :  GPIO 模式
 * @函数返回：无
 * @调用方法：SetGPIO MyGpio(88, OUT);
 * @备注说明：设置 GPIO 模式时，IN 为输入模式，OUT 为输出模式
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
SetGPIO::SetGPIO(uint8_t gpio, const string &dir)
{
    if (this->GpioExport(gpio) != 0)    // 判断对应引脚设备文件是否已被创建
    {
        this->GpioUnexport();           // 若被创建，先删除
        this->GpioExport(gpio);         // 再重新创建
    }
    this->GpioDirection(dir);           // 配置引脚模式
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：SetGPIO::~SetGPIO()
 * @功能说明：GPIO 类的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
SetGPIO::~SetGPIO()
{
    if (this->fd > 0)
        close(this->fd);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetGPIO::GpioExport(uint8_t gpio)
 * @功能说明：导出 GPIO 
 * @参数说明：goio  :  想要使用的 GPIO 引脚号
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：MyGpio.GpioExport(88);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetGPIO::GpioExport(uint8_t gpio)
{
    /* echo xx > /sys/class/gpio/export */
    this->Gpio = gpio;
    this->fd = open("/sys/class/gpio/export", O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "打开 export 文件失败" << endl;
        return -1;
    }
    string str = to_string(gpio);
    if (write(this->fd, str.c_str(), str.size()) != str.size())
    {
        cerr << "导出 gpio" << gpio << " 失败" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetGPIO::GpioUnexport()
 * @功能说明：取消导出 GPIO 
 * @参数说明：无
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：MyGpio.GpioUnexport();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetGPIO::GpioUnexport()
{
    /* echo xx > /sys/class/gpio/unexpor */
    this->fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "打开 unexport 文件失败" << endl;
        return -1;
    }
    string str = to_string(this->Gpio);
    if (write(this->fd, str.c_str(), str.size()) != str.size())
    {
        cerr << "取消导出 gpio" << this->Gpio << " 失败" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetGPIO::GpioDirection(const string &dir)
 * @功能说明：设置 GPIO 工作模式
 * @参数说明：dir  :  GPIO 工作模式
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：MyGpio.GpioDirection(OUT);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetGPIO::GpioDirection(const string &dir)
{
    /* echo "in" > /sys/class/gpio/gpioxx/direction
       echo "out" > /sys/class/gpio/gpioxx/direction */
    string str = "/sys/class/gpio/gpio" + to_string(this->Gpio) + "/direction";
    this->fd = open(str.c_str(), O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "打开 direction 文件失败" << endl;
        return -1;
    }
    if (write(this->fd, dir.c_str(), dir.size()) != dir.size())
    {
        cerr << "设置 gpio 为 " << dir << " 模式失败" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetGPIO::SetGpioValue(uint8_t val)
 * @功能说明：设置 GPIO 电平值
 * @参数说明：val  :  GPIO 电平值
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：MyGpio.SetGpioValue(1);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetGPIO::SetGpioValue(uint8_t val)
{
    /* echo 1 > /sys/class/gpio/gpioxx/value
       echo 0 > /sys/class/gpio/gpioxx/value */
    string str = "/sys/class/gpio/gpio" + to_string(this->Gpio) + "/value";
    // cout << str << " = " << val << endl;
    this->fd = open(str.c_str(), O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "打开value文件失败" << endl;
        return -1;
    }
    int ret = write(this->fd, to_string(val).c_str(), to_string(val).size());
    if (ret != to_string(val).size())
    {
        cerr << "设置gpio电平为" << val << "失败" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetGPIO::GetGpioValue()
 * @功能说明：获取 GPIO 电平值
 * @参数说明：无
 * @函数返回：返回当前 GPIO 电平值
 * @调用方法：int val = MyGpio.GetGpioValue();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetGPIO::GetGpioValue()
{
    /* cat /sys/class/gpio/gpioxx/value */
    string str = "/sys/class/gpio/gpio" + to_string(this->Gpio) + "/value";
    this->fd = open(str.c_str(), O_RDONLY);
    if (this->fd < 0)
    {
        cerr << "打开value文件失败" << endl;
        return -1;
    }
    char val;
    if (read(this->fd, &val, sizeof(val)) != sizeof(val))
    {
        close(this->fd);
        cerr << "读取 GPIO 值失败" << endl;
        return -1;
    }
    close(this->fd);
    return ((val == '1') ? 1 : 0);
}
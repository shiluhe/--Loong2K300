#include "HW_gpio.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HWGpio::HWGpio()
 * @功能说明：硬件配置 GPIO 的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：HWGpio MyGpio;
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWGpio::HWGpio()
{
    this->GPIO_ONE   = NULL;
    this->GPIO_O     = NULL;
    this->GPIO_I     = NULL;
    this->GPIO_REUSE = NULL;
    this->Gpio       = -1;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HWGpio::HWGpio(uint8_t gpio, uint8_t mode) : Gpio(gpio)
 * @功能说明：硬件配置 GPIO 的有参构造函数
 * @参数说明：gpio  :  对应使用的引脚号
 * @参数说明：mode  :  GPIO 模式
 * @函数返回：无
 * @调用方法：HWGpio MyGpio(88, GPIO_Mode_Out);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWGpio::HWGpio(uint8_t gpio, uint8_t mode) : Gpio(gpio)
{
    this->GPIO_ONE = AddressMap(LS_GPIO_BASE_ADDR + LS_GPIO_OEN_OFFSET(gpio), PAGE_SIZE);
    this->GPIO_O   = AddressMap(LS_GPIO_BASE_ADDR + LS_GPIO_O_OFFSET(gpio), PAGE_SIZE);
    this->GPIO_I   = AddressMap(LS_GPIO_BASE_ADDR + LS_GPIO_I_OFFSET(gpio), PAGE_SIZE);
    // 配置引脚复用为 GPIO 模式   88  / 16 = 5
    GpioReuse(this->Gpio, 0b00);
    this->GpioDirection(mode);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HWGpio::GpioDirection(uint8_t mode)
 * @功能说明：硬件配置 GPIO 的模式
 * @参数说明：mode  :  GPIO 模式
 * @函数返回：无
 * @调用方法：MyGpio.GpioDirection(GPIO_Mode_Out);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWGpio::GpioDirection(uint8_t mode)
{
    if(mode == 1){
        LS_writel(this->GPIO_ONE, LS_readl(this->GPIO_ONE) | BIT(this->Gpio % 8));
    }else if(mode == 0){
        LS_writel(this->GPIO_ONE, LS_readl(this->GPIO_ONE) & ~BIT(this->Gpio % 8));
    }else
        cout << "模式设置错误" << endl;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void HWGpio::SetGpioValue(uint8_t value)
 * @功能说明：硬件配置 GPIO 的输出值
 * @参数说明：value  :  GPIO 输出值
 * @函数返回：无
 * @调用方法：MyGpio.SetGpioValue(1);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HWGpio::SetGpioValue(uint8_t value)
{
    if(value == 1){
        LS_writel(this->GPIO_O, LS_readl(this->GPIO_O) | BIT(this->Gpio % 8));
    }else if(value == 0){
        LS_writel(this->GPIO_O, LS_readl(this->GPIO_O) & ~BIT(this->Gpio % 8));
    }else
        cout << "输出值设置失败" << endl;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：bool HWGpio::GetGpioValue()
 * @功能说明：硬件配置 GPIO 的输出值
 * @参数说明：无
 * @函数返回：无
 * @调用方法：int val = MyGpio.GetGpioValue(1);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
bool HWGpio::GetGpioValue()
{
    return ((LS_readl(this->GPIO_I) & BIT(this->Gpio % 8)) == BIT(this->Gpio % 8));
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：HWGpio::~HWGpio()
 * @功能说明：硬件配置 GPIO 的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HWGpio::~HWGpio()
{
    munmap(this->GPIO_ONE, PAGE_SIZE);
    munmap(this->GPIO_O, PAGE_SIZE);
    munmap(this->GPIO_I, PAGE_SIZE);
    munmap(this->GPIO_REUSE, PAGE_SIZE);
}
#include "PWM_Encoder.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：LS_PwmEncoder::LS_PwmEncoder(int channel, int gpio) :
                DirGPIO(gpio, GPIO_Mode_In)
 * @功能说明：配置编码器类的有参构造函数
 * @参数说明：channel  :  PWM 的通道编号
 * @参数说明：gpio     :  检测编码器方向的引脚编号
 * @函数返回：无
 * @调用方法：encoder.Init();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_PwmEncoder::LS_PwmEncoder(int channel, int gpio) : DirGPIO(gpio, GPIO_Mode_In)
{
    this->PWM_ADDR = LS_PWM_BASE_ADDR + channel * LS_PWM_OFFSET;

    this->CTRL = AddressMap(this->PWM_ADDR + LS_PWM_CTRL_OFFSET, PAGE_SIZE);
    this->Low_buffer = AddressMap(this->PWM_ADDR + LS_PWM_LOW_BUFFER_OFFSET, PAGE_SIZE);
    this->Full_buffer = AddressMap(this->PWM_ADDR + LS_PWM_FULL_BUFFER_OFFSET, PAGE_SIZE);
    this->Init();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：LS_PwmEncoder::~LS_PwmEncoder(void)
 * @功能说明：配置编码器类的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_PwmEncoder::~LS_PwmEncoder(void)
{
    this->DirGPIO.~HWGpio();
    munmap(this->CTRL, PAGE_SIZE);
    munmap(this->Low_buffer,  PAGE_SIZE);
    munmap(this->Full_buffer, PAGE_SIZE);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void LS_PwmEncoder::Init(void)
 * @功能说明：初始化 PWM 控制器为计数模式
 * @参数说明：无
 * @函数返回：无
 * @调用方法：encoder.Init();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_PwmEncoder::Init(void)
{
    LS_writel(this->CTRL, 0);
    uint32_t reg = 0 | LS_PWM_CTRL_EN | LS_PWM_CTRL_CAPTE | LS_PWM_CTRL_INTE;
    LS_writel(this->CTRL, LS_readl(this->CTRL) | reg);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void LS_PwmEncoder::ResetCounter(void)
 * @功能说明：重置计数器操作
 * @参数说明：无
 * @函数返回：无
 * @调用方法：encoder.ResetCounter();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_PwmEncoder::ResetCounter(void)
{
    LS_writel(this->CTRL, LS_readl(this->CTRL) | LS_PWM_CTRL_RST);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void LS_PwmEncoder::CloseResCounter(void)
 * @功能说明：关闭重置计数器操作
 * @参数说明：无
 * @函数返回：无
 * @调用方法：encoder.CloseResCounter();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_PwmEncoder::CloseResCounter(void)
{
    LS_writel(this->CTRL, LS_readl(this->CTRL) & ~LS_PWM_CTRL_RST);
}

/*
    N : 旋转编码器线数
    t ：两个脉冲间的脉宽
    r ：旋转轴的半径
    w = 2π/(N*t) : 旋转速度
*/
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：float LS_PwmEncoder::Update(void)
 * @功能说明：获取编码器当前瞬时速度(带方向)
 * @参数说明：无
 * @函数返回：当前瞬时速度
 * @调用方法：float val = encoder.Update();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
float LS_PwmEncoder::Update(void)
{
    uint32_t val = LS_readl(this->Full_buffer);
    (val == 0) ? (val = 1) : (val); // 判断读取是否为 0，防止除法时程序崩溃
    return (float)(100000000.0 / val / NUM_ENCODER_LINE*(this->DirGPIO.GetGpioValue() * 2 - 1));
}
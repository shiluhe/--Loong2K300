#include "gtim_pwm.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：GtimPwm::GtimPwm()
 * @功能说明：GTIM 控制器 PWM 模式的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：GtimPwm MyPwm;
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
GtimPwm::GtimPwm()
{

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：GtimPwm::GtimPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux)
                 : Gpio(gpio), Pol(pol), Period(period), Duty_cycle(duty_cycle), ChNum(chNum - 1)
 * @功能说明：GTIM 控制器 PWM 模式的有参构造函数
 * @参数说明：gpio        :  输出 PWM 的 GPIO 引脚号
 * @参数说明：chNum       :  参数一对应的通道号
 * @参数说明：pol         :  PWM 输出极性
 * @参数说明：period      :  PWM 周期
 * @参数说明：duty_cycle  :  PWM 占空比
 * @参数说明：mux         :  复用 GPIO 引脚
 * @函数返回：无
 * @调用方法：GtimPwm MyPwm(88, 2, LS_GTIM_INVERSED, 2000000 - 1, 150000);
 * @备注说明：参数六设置有默认值，可以不修改，GPIO 对应的通道号可查看：
 *           《龙芯2K0300处理器用户手册》
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
GtimPwm::GtimPwm(uint8_t gpio, uint8_t chNum, uint8_t pol, uint32_t period, uint32_t duty_cycle, int mux)
    : Gpio(gpio), Pol(pol), Period(period), Duty_cycle(duty_cycle), ChNum(chNum - 1)
{
    // 配置功能复用
    GpioReuse(this->Gpio, mux);
    // 初始化所有寄存器
    LS_writel(AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_EGR, PAGE_SIZE), 0x01);
    // 启动计数器
    LS_writel(AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_CR1, PAGE_SIZE), 0x01);
    // 获取虚拟地址
    this->GTIM_ARR     = AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_ARR, PAGE_SIZE);
    this->GTIM_CCRx    = AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_CCR1 + this->ChNum * LS_GTIM_CCRx_OFS, PAGE_SIZE);
    this->GTIM_CCMR[0] = AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_CCMR1, PAGE_SIZE);
    this->GTIM_CCMR[1] = AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_CCMR2, PAGE_SIZE);
    this->GTIM_CCER    = AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_CCER, PAGE_SIZE);
    this->GTIM_CNT     = AddressMap(LS_GTIM_BASE_ADDR + LS_GTIM_CNT, PAGE_SIZE);
    // 设置 PWM 模式，默认为模式 2
    this->SetMode(LS_GTIM_Mode_2);
    // 设置极性
    this->SetPolarity(this->Pol);
    // 设置周期
    this->SetPeriod(this->Period);
    // 设置占空比
    this->SetDutyCycle(this->Duty_cycle);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：GtimPwm::~GtimPwm(void)
 * @功能说明：GTIM 控制器 PWM 模式的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
GtimPwm::~GtimPwm(void)
{
    munmap(this->GTIM_CCMR[0], PAGE_SIZE);
    munmap(this->GTIM_CCMR[1], PAGE_SIZE);
    munmap(this->GTIM_ARR, PAGE_SIZE);
    munmap(this->GTIM_CCRx, PAGE_SIZE);
    munmap(this->GTIM_CCER, PAGE_SIZE);
    munmap(this->GTIM_CNT, PAGE_SIZE);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GtimPwm::Enable(void)
 * @功能说明：使能 PWM
 * @参数说明：无
 * @函数返回：无
 * @调用方法：MyPwm.Enable();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwm::Enable(void)
{
    LS_writel(this->GTIM_CCER, LS_readl(this->GTIM_CCER) | (0x1 << (this->ChNum * 4 + 0)));
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GtimPwm::Disable(void)
 * @功能说明：失能 PWM
 * @参数说明：无
 * @函数返回：无
 * @调用方法：MyPwm.Disable();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwm::Disable(void)
{
    LS_writel(this->GTIM_CCER, LS_readl(this->GTIM_CCER) & ~(0x1 << (this->ChNum * 4 + 0)));
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GtimPwm::SetPolarity(uint8_t pol)
 * @功能说明：设置 PWM 极性
 * @参数说明：pol  :  所设置的极性
 * @函数返回：无
 * @调用方法：MyPwm.SetPolarity(LS_GTIM_INVERSED);
 * @备注说明：.h 文件中的宏，设置 LS_GTIM_NORMAL 为正常输出，设置 LS_GTIM_INVERSED 为反相输出
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwm::SetPolarity(uint8_t pol)
{
    // 配置 chNum 的输出极性
    uint32_t reg = LS_readl(this->GTIM_CCER) & ~(0x1 << (this->ChNum * 4 + 1)) | (pol << (this->ChNum * 4 + 1));
    LS_writel(this->GTIM_CCER, reg);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GtimPwm::SetPeriod(uint32_t cycle)
 * @功能说明：设置 PWM 周期
 * @参数说明：cycle  :  周期值
 * @函数返回：无
 * @调用方法：MyPwm.SetPeriod(2000000);
 * @备注说明：GTIM 控制器时钟为 100M，假设要设置 50Hz 周期，那该参数应填：
 *           100000000 / 50 = 2000000
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwm::SetPeriod(uint32_t cycle)
{
    this->Period = cycle;
    LS_writel(this->GTIM_ARR, this->Period);
    LS_writel(this->GTIM_CNT, 0);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GtimPwm::SetDutyCycle(uint32_t duty_cycle)
 * @功能说明：设置 PWM 占空比
 * @参数说明：duty_cycle  :  占空比
 * @函数返回：无
 * @调用方法：MyPwm.SetDutyCycle(150000);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwm::SetDutyCycle(uint32_t duty_cycle)
{
    this->Duty_cycle = duty_cycle;
    LS_writel(this->GTIM_CCRx, this->Duty_cycle);
    LS_writel(this->GTIM_CNT, 0);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void GtimPwm::SetMode(uint8_t mode)
 * @功能说明：设置 PWM 模式
 * @参数说明：mode  :  设置的模式
 * @函数返回：无
 * @调用方法：MyPwm.SetMode(LS_GTIM_Mode_2);
 * @备注说明：设置 LS_GTIM_Mode_1 为模式 1，设置 LS_GTIM_Mode_2 为模式 2
 *           默认设置为模式 2
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GtimPwm::SetMode(uint8_t mode)
{
    this->Mode = mode;
    // 配置 chNum 的 PWM 模式
    uint32_t reg = LS_readl(this->GTIM_CCMR[this->ChNum / 2]) & ~(0x7 << (ChNum % 2 * 8 + 4)) | (mode << (ChNum % 2 * 8 + 4));
    // 配置 chNum 的 PWM 模式
    LS_writel(this->GTIM_CCMR[ChNum / 2], reg);
}
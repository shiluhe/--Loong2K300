#include "pwm.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：SetPWM::SetPWM()
 * @功能说明：配置 PWM 的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：无
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
SetPWM::SetPWM()
{
    this->fd = -1;
    this->Mold = "";
    this->Cycle = 0;
    this->Channel = "";
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：SetPWM::SetPWM(const string &mold, int cycle, int duty_cycle, const string &polarity) :
                Mold(mold), Cycle(cycle), Duty_Cycle(duty_cycle), Polarity(polarity)
 * @功能说明：配置 PWM 的有参构造函数
 * @参数说明：mold       :  PWM 编号
 * @参数说明：cycle      :  周期
 * @参数说明：duty_cycle :  占空比
 * @参数说明：polarity   :  极性
 * @函数返回：无
 * @调用方法：SetPWM pwm1(PWM1, 10000, 5000, "inversed");
 * @备注说明：极性设置 "normal" 为正常输出，设置 "inversed" 为反相输出
 *           设置周期时计算公式为 周期 = 1000000000 / 设置值
 *           比如设置的值为 100000，最后得到的 PWM 就是 10000Hz
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
SetPWM::SetPWM(const string &mold, int cycle, int duty_cycle, const string &polarity) : 
            Mold(mold), Cycle(cycle), Duty_Cycle(duty_cycle), Polarity(polarity)
{
    if (this->ExportPWM(mold) != 0)
    {
        this->UnexportPWM();
        this->ExportPWM(mold);
    }
    this->SetPeriod(cycle);
    this->SetDutyCycle(duty_cycle);
    this->SetPolarity(polarity);
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：SetPWM::~SetPWM()
 * @功能说明：配置 PWM 的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
SetPWM::~SetPWM()
{
    // 失能pwm
    this->Disable();
    // 关闭pwm设备文件
    this->UnexportPWM();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetPWM::ExportPWM(const string &mold)
 * @功能说明：配置 mold 选择的 PWM 通道文件
 * @参数说明：mold  :  PWM 编号
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：pwm1.ExportPWM(PWM1);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetPWM::ExportPWM(const string &mold)
{
    /* echo 0 > /sys/clear/pwm/pwmchip0/export */
    this->Channel = "0";
    this->Mold = mold;
    // 打开pwm设备文件
    string str = "/sys/class/pwm/" + mold + "/export";
    this->fd = open(str.c_str(), O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "无法打开pwm设备导出文件" << endl;
        return -1;
    }
    // 写入channel，打开通道
    int ret = write(this->fd, this->Channel.c_str(), this->Channel.size());
    if (ret != this->Channel.size())
    {
        cerr << "无法导出pwm通道" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetPWM::UnexportPWM()
 * @功能说明：关闭 mold 选择的 PWM 通道文件
 * @参数说明：无
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：pwm1.UnexportPWM();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetPWM::UnexportPWM()
{
    this->Channel = "0";
    // 打开pwm设备文件
    string str = "/sys/class/pwm/" + this->Mold + "/unexport";
    this->fd = open(str.c_str(), O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "无法打开pwm关闭导出文件" << endl;
        return -1;
    }
    // 关闭通道
    int ret = write(this->fd, this->Channel.c_str(), this->Channel.size());
    if (ret != this->Channel.size())
    {
        cerr << "无法关闭pwm通道" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetPWM::SetPeriod(int cycle)
 * @功能说明：设置周期
 * @参数说明：cycle  :  周期值
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：pwm1.SetPeriod(100000);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetPWM::SetPeriod(int cycle)
{
    string str = "/sys/class/pwm/" + this->Mold + "/pwm" + this->Channel + "/period";
    this->fd = open(str.c_str(), O_RDWR);
    if (this->fd < 0)
    {
        cerr << "无法打开pwm周期设备文件" << endl;
        return -1;
    }
    str.clear();
    str = to_string(cycle);
    int ret = write(this->fd, str.c_str(), str.size());
    if (ret != str.size())
    {
        cerr << "无法设置pwm周期" << endl;
        close(this->fd);
        return -1;
    }
    this->Cycle = cycle;
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetPWM::SetDutyCycle(int duty_cycle)
 * @功能说明：设置占空比
 * @参数说明：duty_cycle  :  占空比
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：pwm1.SetDutyCycle(10000);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetPWM::SetDutyCycle(int duty_cycle)
{
    /* echo duty_cycle > /sys/clear/pwm/pwmchip0/pwm0/duty_cycle */
    string str = "/sys/class/pwm/" + this->Mold + "/pwm" + this->Channel + "/duty_cycle";
    this->fd = open(str.c_str(), O_RDWR);
    if (this->fd < 0)
    {
        cerr << "无法打开 pwm 占空比设备文件" << endl;
        return -1;
    }
    str.clear();
    str = to_string(duty_cycle);
    int ret = write(this->fd, str.c_str(), str.size());
    if (ret != str.size())
    {
        cerr << "无法设置pwm占空比" << endl;
        close(this->fd);
        return -1;
    }
    this->Duty_Cycle = duty_cycle;
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetPWM::SetPolarity(const string &polarity)
 * @功能说明：设置极性
 * @参数说明：polarity  :  极性
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：pwm1.SetPolarity("inversed");
 * @备注说明：设置 "normal" 为正常输出，设置 "inversed" 为反相输出
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetPWM::SetPolarity(const string &polarity)
{
    /* echo "normal" > /sys/clear/pwm/pwmchip0/pwm0/polarity */
    string str = "/sys/class/pwm/" + this->Mold + "/pwm" + this->Channel + "/polarity";
    this->fd = open(str.c_str(), O_RDWR);
    if (this->fd < 0)
    {
        cerr << "无法打开pwm极性设备文件" << endl;
        return -1;
    }
    str.clear();
    int ret = write(this->fd, polarity.c_str(), polarity.size());
    if (ret != polarity.size())
    {
        cerr << "无法设置pwm极性" << endl;
        close(this->fd);
        return -1;
    }
    this->Polarity = polarity;
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetPWM::Enable()
 * @功能说明：使能 PWM
 * @参数说明：无
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：pwm1.Enable();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetPWM::Enable()
{
    /* echo 1 > /sys/clear/pwm/pwmchip0/pwm0/enable */
    string str = "/sys/class/pwm/" + this->Mold + "/pwm" + this->Channel + "/enable";
    this->fd = open(str.c_str(), O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "无法打开使能设备文件" << endl;
        close(this->fd);
        return -1;
    }
    int ret = write(this->fd, "1", 1);
    if (ret != 1)
    {
        cerr << "无法使能pwm通道" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int SetPWM::Disable()
 * @功能说明：失能 PWM
 * @参数说明：无
 * @函数返回：成功返回 0，失败返回 -1
 * @调用方法：pwm1.Disable();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SetPWM::Disable()
{
    /* echo 0 > /sys/clear/pwm/pwmchip0/pwm0/enable */
    string str = "/sys/class/pwm/" + this->Mold + "/pwm" + this->Channel + "/enable";
    this->fd = open(str.c_str(), O_WRONLY);
    if (this->fd < 0)
    {
        cerr << "无法打开使能设备文件" << endl;
        close(this->fd);
        return -1;
    }
    int ret = write(this->fd, "0", 1);
    if (ret != 1)
    {
        cerr << "无法失能pwm通道" << endl;
        close(this->fd);
        return -1;
    }
    close(this->fd);
    return 0;
}
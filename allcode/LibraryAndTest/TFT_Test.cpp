#include "TFT.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void TFTTest()
 * @功能说明：TFT 屏幕测试程序
 * @参数说明：无
 * @函数返回：无
 * @调用方法：TFTTest();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TFTTest()
{
    TFTSPI_Init(0); // LCD初始化    0：横屏     1：竖屏
    cout << "TFTSPI_Init" << endl;
    TFTSPI_CLS(u16BLUE);    // 蓝色屏幕
    cout << "TFTSPI_CLS" << endl;
    TFTSPI_P8X16Str(0, 2, (char*)"Beijing Longqiu", u16RED, u16BLUE);   // 字符串显示
    TFTSPI_P8X16Str(0, 4, (char*)"Long Qiu i.s.t.", u16WHITE, u16BLACK);   // 字符串显示
    cout << "TFTSPI_P8x16Str" << endl;
    sleep(1);
    char txt[32];
    unsigned short count = 1;
    while(1) 
    {
        memset(txt, 0, sizeof(txt));
        sprintf(txt, "variate:%05d", count);
        TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLUE);
        usleep(500000);
        count++;
    }
}

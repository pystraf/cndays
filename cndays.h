#pragma once
/*头文件：cndays.h;对应的实现文件：cndays.cpp
本文件的实现，参考自多地个地方。我也记不清，如果你觉得
对你的著作权有损，请联系我需要我删除的请联系我：wx:domics
qq:1176708259。我收到信息后立即删除本博。谢谢！
如果没意见的话，就共大家分享吧！感激！*/

#include <tchar.h>
#include <string>
#include <math.h>
#include <iostream>
// 来自网络的即说是天文观测数据。大意：
// 0~4 共5bit 春节日份
// 5~6 共2bit 春节月份
// 7~19 共13bit 13个月的大小月情况(如果无闰月，最后位无效)，大月为1,小月为0
// 20~23 共4bit 记录闰月的月份，如果没有闰月为0

using namespace std;

static const unsigned int BaseData[199] = {

    0x04AE53, 0x0A5748, 0x5526BD, 0x0D2650, 0x0D9544,
    0x46AAB9, 0x056A4D, 0x09AD42, 0x24AEB6, 0x04AE4A, // 1901年-1910年

    0x6A4DBE, 0x0A4D52, 0x0D2546, 0x5D52BA, 0x0B544E,
    0x0D6A43, 0x296D37, 0x095B4B, 0x749BC1, 0x049754, // 1911年-1920年

    0x0A4B48, 0x5B25BC, 0x06A550, 0x06D445, 0x4ADAB8,
    0x02B64D, 0x095742, 0x2497B7, 0x04974A, 0x664B3E, // 1921年-1930年

    0x0D4A51, 0x0EA546, 0x56D4BA, 0x05AD4E, 0x02B644,
    0x393738, 0x092E4B, 0x7C96BF, 0x0C9553, 0x0D4A48, // 1931年-1940年

    0x6DA53B, 0x0B554F, 0x056A45, 0x4AADB9, 0x025D4D,
    0x092D42, 0x2C95B6, 0x0A954A, 0x7B4ABD, 0x06CA51, // 1941年-1950年

    0x0B5546, 0x555ABB, 0x04DA4E, 0x0A5B43, 0x352BB8,
    0x052B4C, 0x8A953F, 0x0E9552, 0x06AA48, 0x7AD53C, // 1951年-1960年

    0x0AB54F, 0x04B645, 0x4A5739, 0x0A574D, 0x052642,
    0x3E9335, 0x0D9549, 0x75AABE, 0x056A51, 0x096D46, // 1961年-1970年

    0x54AEBB, 0x04AD4F, 0x0A4D43, 0x4D26B7, 0x0D254B,
    0x8D52BF, 0x0B5452, 0x0B6A47, 0x696D3C, 0x095B50, // 1971年-1980年

    0x049B45, 0x4A4BB9, 0x0A4B4D, 0xAB25C2, 0x06A554,
    0x06D449, 0x6ADA3D, 0x0AB651, 0x093746, 0x5497BB, // 1981年-1990年

    0x04974F, 0x064B44, 0x36A537, 0x0EA54A, 0x86B2BF,
    0x05AC53, 0x0AB647, 0x5936BC, 0x092E50, 0x0C9645, // 1991年-2000年

    0x4D4AB8, 0x0D4A4C, 0x0DA541, 0x25AAB6, 0x056A49,
    0x7AADBD, 0x025D52, 0x092D47, 0x5C95BA, 0x0A954E, // 2001年-2010年

    0x0B4A43, 0x4B5537, 0x0AD54A, 0x955ABF, 0x04BA53,
    0x0A5B48, 0x652BBC, 0x052B50, 0x0A9345, 0x474AB9, // 2011年-2020年

    0x06AA4C, 0x0AD541, 0x24DAB6, 0x04B64A, 0x69573D,
    0x0A4E51, 0x0D2646, 0x5E933A, 0x0D534D, 0x05AA43, // 2021年-2030年

    0x36B537, 0x096D4B, 0xB4AEBF, 0x04AD53, 0x0A4D48,
    0x6D25BC, 0x0D254F, 0x0D5244, 0x5DAA38, 0x0B5A4C, // 2031年-2040年

    0x056D41, 0x24ADB6, 0x049B4A, 0x7A4BBE, 0x0A4B51,
    0x0AA546, 0x5B52BA, 0x06D24E, 0x0ADA42, 0x355B37, // 2041年-2050年

    0x09374B, 0x8497C1, 0x049753, 0x064B48, 0x66A53C,
    0x0EA54F, 0x06B244, 0x4AB638, 0x0AAE4C, 0x092E42, // 2051年-2060年

    0x3C9735, 0x0C9649, 0x7D4ABD, 0x0D4A51, 0x0DA545,
    0x55AABA, 0x056A4E, 0x0A6D43, 0x452EB7, 0x052D4B, // 2061年-2070年

    0x8A95BF, 0x0A9553, 0x0B4A47, 0x6B553B, 0x0AD54F,
    0x055A45, 0x4A5D38, 0x0A5B4C, 0x052B42, 0x3A93B6, // 2071年-2080年

    0x069349, 0x7729BD, 0x06AA51, 0x0AD546, 0x54DABA,
    0x04B64E, 0x0A5743, 0x452738, 0x0D264A, 0x8E933E, // 2081年-2090年

    0x0D5252, 0x0DAA47, 0x66B53B, 0x056D4F, 0x04AE45,
    0x4A4EB9, 0x0A4D4C, 0x0D1541, 0x2D92B5 // 2091年-2099年

};

typedef struct structDate
{
    // 农历部分结构
    string cnYear = "";  // 中文的年份(天干地支+生肖年)
    string cnMonth = ""; // 中文月份(腊月至正月)
    string cnDay = "";   // 初一至三十
    bool leap = false;   // 是否闰月

    // 公历部分结构：返回值其实是农历日期的公历书写形式。
    int year = 0;  // 年
    int month = 0; // 月
    int day = 0;   // 日

} Date, cnDate; // 公历、农历同一结构中

/*xh:小寒的首字母，其它的类推：用首字母方便记。
0小寒1大寒2立春3雨水4惊蛰5春分6清明7谷雨
8立夏9小满10芒种11夏至12小暑13大暑14立秋15处暑
16白露17秋分18寒露19霜降20立冬21小雪22大雪23冬至*/
const int xh = 0;
const int dh = 1;
const int lc = 2;
const int ys = 3;
const int jz = 4;
const int cf = 5;
const int qm = 6;
const int gy = 7;
const int lx = 8;
const int xm = 9;
const int mz = 10;
const int xz = 11;
const int xs = 12;
const int ds = 13;
const int lq = 14;
const int cs = 15;
const int bl = 16;
const int qf = 17;
const int hl = 18;
const int sj = 19;
const int ld = 20;
const int xx = 21;
const int dx = 22;
const int dz = 23;

// 类
class CCnDays
{

public:
    cnDate GetLunar(int yyyy, int mm, int dd); // 公历转农历

    Date GetSolar(cnDate lunar); // 农历转公历

    Date GetDays24(int year, int index); // 获取24节气的公历日期

    Date DaysAddOrSubtrack(Date date1, int days, bool bAdd = false); // date1 + days Or date1-days

    bool IsLeapYear(int year); // 判断给定的年份是不是闰年

    string cn24Days[24] =
        {
            "小寒", "大寒", "立春", "雨水",
            "惊蛰", "春分", "清明", "谷雨",
            "立夏", "小满", "芒种", "夏至",
            "小暑", "大暑", "立秋", "处暑",
            "白露", "秋分", "寒露", "霜降",
            "立冬", "小雪", "大雪", "冬至"};

    string cn24DaysOnMonth[12] =
        {
            "大寒", "雨水", "春分", "谷雨",
            "小满", "夏至", "大暑", "处暑",
            "秋分", "霜降", "小雪", "冬至"};

    string cnMonth[12] =
        {
            "正月", "二月", "三月", "四月",
            "五月", "六月", "七月", "八月",
            "九月", "十月", "冬月", "腊月"};

    string cnDays[30] =
        {
            "初一", "初二", "初三", "初四",
            "初五", "初六", "初七", "初八",
            "初九", "初十", "十一", "十二",
            "十三", "十四", "十五", "十六",
            "十七", "十八", "十九", "二十",
            "廿一", "廿二", "廿三", "廿四",
            "廿五", "廿六", "廿七", "廿八",
            "廿九", "三十"};

protected:
    int monthTotal[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}; // 公农历转换时用

    int DaysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // 每个月的天数，内部使用

    int GetDaysOfMonth(int year, int month); // 获取每月的天数

    Date DaysToDate(int ndays); // 将GetDays24()函数返回值转换成月日

    cnDate GetLunarPart(int yyyy, int mm, int dd); // 公历转农历
};

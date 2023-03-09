
#include "cndays.h"
#include <ctime>
#include <cassert>

using namespace std;

cnDate CCnDays::GetLunar(int yyyy, int mm, int dd)
{
    cnDate lunar = GetLunarPart(yyyy, mm, dd);

    // 再计算农历年的天干地支与生肖年：
    string sx[12] =
        {
            "鼠", "牛", "虎", "兔", "龙", "蛇",
            "马", "羊", "猴", "鸡", "狗", "猪"};

    /*甲、乙、丙、丁、戊、己、庚、辛、壬、癸被称为“十天干”，
    子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥叫作“十二地支”*/

    string tg[10] =
        {
            "甲", "乙", "丙", "丁", "戊",
            "己", "庚", "辛", "壬", "癸"};

    /*甲、乙、丙、丁、戊、己、庚、辛、壬、癸被称为“十天干”，
   子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥叫作“十二地支”*/

    string dz[12] =
        {

            "子", "丑", "寅", "卯", "辰", "巳",
            "午", "未", "申", "酉", "戌", "亥"

        };

    lunar.cnYear = tg[(lunar.year - 1864) % 10];  // 天干
    lunar.cnYear += dz[(lunar.year - 1864) % 12]; // 地支
    // lunar.cnYear += "【" + sx[(lunar.year - 1900) % 12] + "】年"; // 生肖
    lunar.cnYear += "年"; // 生肖

    // 格成农历月
    if (lunar.leap)
    {
        Date ld1, ld2 = lunar;
        ld1.year = yyyy;
        ld1.month = mm;
        ld1.day = dd;

        // 农历前一个月的最后一天的公历日期
        ld2 = DaysAddOrSubtrack(ld1, ld2.day, false);

        // 农历前一个月的最后一天的农历日期
        ld1 = GetLunarPart(ld2.year, ld2.month, ld2.day);

        if (lunar.month == ld1.month)
        {
            lunar.cnMonth = "闰" + cnMonth[lunar.month - 1];
        }
        else
        {
            lunar.cnMonth = cnMonth[lunar.month - 1];
        }
    }
    else
    {
        lunar.cnMonth = cnMonth[lunar.month - 1];
    }

    // 格成农历日
    lunar.cnDay = cnDays[lunar.day - 1];

    return lunar;
}

Date CCnDays::GetSolar(cnDate lunar)
{

    int year = lunar.year,
        month = lunar.month,
        day = lunar.day;
    int byNow, xMonth, i;
    Date solar;
    byNow = (BaseData[year - 1901] & 0x001F) - 1;
    if (((BaseData[year - 1901] & 0x0060) >> 5) == 2)
        byNow += 31;
    for (i = 1; i < month; i++)
    {
        if ((BaseData[year - 1901] & (0x80000 >> (i - 1))) == 0)
        {
            byNow += 29;
        }
        else
            byNow += 30;
    }
    byNow += day;
    xMonth = (BaseData[year - 1901] & 0xf00000) >> 20;
    if (xMonth != 0)
    {
        if (month > xMonth || (month == xMonth && lunar.leap))
        {
            if ((BaseData[year - 1901] & (0x80000 >> (month - 1))) == 0)
                byNow += 29;
            else
                byNow += 30;
        }
    }
    if (byNow > 366 || (year % 4 != 0 && byNow == 365))
    {
        year += 1;
        if (year % 4 == 0)
            byNow -= 366;
        else
            byNow -= 365;
    }
    for (i = 1; i <= 13; i++)
    {
        if (monthTotal[i] >= byNow)
        {
            month = i;
            break;
        }
    }
    solar.day = byNow - monthTotal[month - 1];
    solar.month = month;
    solar.year = year;

    return solar;
}

// GetDays24(int year, int index)与DyasOneToDate(int nCnDayOne)
// 结合起来计算指定24节气在公历中的日期=======================================

Date CCnDays::GetDays24(int year, int index)
{
    /*来自网络经验公式，至2099年有效*/
    int y = year - 1900;

    int ymd = 365.242 * y + 6.2 + 15.22 * index - 1.9 * sin(0.262 * index);

    Date rd = DaysToDate(ymd);

    rd.year = year;

    return rd;
} /*//GetDays24(int year, int index)与DyasOneToDate(int nCnDayOne)
 //结合起来计算指定24节气在公历中的日期
 =========================================================================*/

bool CCnDays::IsLeapYear(int year)
{
    if (year % 400 == 0)
    {
        return true;
    }
    else if (year % 100 == 0)
    {
        return false;
    }
    else if (year % 4 == 0)
    {
        return true;
    }
    return false;
}

Date CCnDays::DaysToDate(int ndays)
{
    int m = 0;
    int y = 1900;
    Date d;

    while (ndays > GetDaysOfMonth(y, m))
    {
        ndays = ndays - GetDaysOfMonth(y, m);
        m = (m + 1) % 12;

        if (m == 0)
            y += 1;

        d.year = y;
        d.month = m + 1;
        d.day = ndays;
    }

    return d;
}

cnDate CCnDays::GetLunarPart(int yyyy, int mm, int dd)
{
    int year = yyyy, month = mm, day = dd;
    int bySpring, bySolar, daysPerMonth;
    int index, flag;
    cnDate plunar;

    // bySpring 记录春节离当年元旦的天数。
    // bySolar 记录阳历日离当年元旦的天数。
    if (((BaseData[year - 1901] & 0x0060) >> 5) == 1)
        bySpring = (BaseData[year - 1901] & 0x001F) - 1;
    else
        bySpring = (BaseData[year - 1901] & 0x001F) - 1 + 31;
    bySolar = monthTotal[month - 1] + day - 1;
    if ((!(year % 4)) && (month > 2))
        bySolar++;

    // daysPerMonth记录大小月的天数 29 或30
    // index 记录从哪个月开始来计算。
    // flag 是用来对闰月的特殊处理。

    // 判断阳历日在春节前还是春节后
    if (bySolar >= bySpring)
    { // 阳历日在春节后（含春节那天）
        bySolar -= bySpring;
        month = 1;
        index = 1;
        flag = 0;
        if ((BaseData[year - 1901] & (0x80000 >> (index - 1))) == 0)
            daysPerMonth = 29;
        else
            daysPerMonth = 30;
        while (bySolar >= daysPerMonth)
        {
            bySolar -= daysPerMonth;
            index++;
            if (month == ((BaseData[year - 1901] & 0xF00000) >> 20))
            {
                flag = ~flag;
                if (flag == 0)
                    month++;
            }
            else
                month++;
            if ((BaseData[year - 1901] & (0x80000 >> (index - 1))) == 0)
                daysPerMonth = 29;
            else
                daysPerMonth = 30;
        }
        day = bySolar + 1;
    }
    else
    { // 阳历日在春节前
        bySpring -= bySolar;
        year--;
        month = 12;
        if (((BaseData[year - 1901] & 0xF00000) >> 20) == 0)
            index = 12;
        else
            index = 13;
        flag = 0;
        if ((BaseData[year - 1901] & (0x80000 >> (index - 1))) == 0)
            daysPerMonth = 29;
        else
            daysPerMonth = 30;
        while (bySpring > daysPerMonth)
        {
            bySpring -= daysPerMonth;
            index--;
            if (flag == 0)
                month--;
            if (month == ((BaseData[year - 1901] & 0xF00000) >> 20))
                flag = ~flag;
            if ((BaseData[year - 1901] & (0x80000 >> (index - 1))) == 0)
                daysPerMonth = 29;
            else
                daysPerMonth = 30;
        }

        day = daysPerMonth - bySpring + 1;
    }
    plunar.day = day;
    plunar.month = month;
    plunar.year = year;
    if (month == ((BaseData[year - 1901] & 0xF00000) >> 20))
        plunar.leap = true;
    else
        plunar.leap = false;

    return plunar;
}

Date CCnDays::DaysAddOrSubtrack(Date date1, int days, bool bAdd)
{
    if (bAdd) // 真为加，
    {
        /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        days += date1.day;

        if (IsLeapYear(date1.year)) // 闰年
        {
            /*rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr*/
            if (date1.month == 2)
            {
                if (days > 29)
                {
                    days = days % 29;
                    date1.month += (days / 29);
                    if (date1.month > 12)
                        date1.year += (date1.month / 12);
                }
            }
            else if (date1.month == 1 || date1.month == 3 || date1.month == 5 || date1.month == 7 || date1.month == 8 || date1.month == 10 || date1.month == 12) // 大月
            {
                if (days > 31)
                {
                    days = days % 31;
                    date1.month += (days % 31);
                    if (date1.month > 12)
                        date1.year += (date1.month / 12);
                }
            }
            else if (date1.month == 4 || date1.month == 6 || date1.month == 9 || date1.month == 11) // 小月
            {
                if (days > 30)
                {
                    days = days % 30;
                    date1.month += (days % 30);
                    if (date1.month > 12)
                        date1.year += (date1.month / 12);
                }
            }
            /*rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr*/
        }
        else // 平年
        {
            /*pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp*/

            if (date1.month == 2)
            {
                if (days > 28)
                {
                    days = days % 28;
                    date1.month += (days % 28);
                    if (date1.month > 12)
                        date1.year += (date1.month / 12);
                }
            }
            else if (date1.month == 1 || date1.month == 3 || date1.month == 5 || date1.month == 7 || date1.month == 8 || date1.month == 10 || date1.month == 12) // 大月
            {
                if (days > 31)
                {
                    days = days % 31;
                    date1.month += (days % 31);
                    if (date1.month > 12)
                        date1.year += (date1.month / 12);
                }
            }
            else if (date1.month == 4 || date1.month == 6 || date1.month == 9 || date1.month == 11) // 小月
            {
                if (days > 30)
                {
                    days = days % 30;
                    date1.month += (days % 30);
                    if (date1.month > 12)
                        date1.year += (date1.month / 12);
                }
            }

            /*pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp*/
        }

        /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    }
    else // 假为减法
    {
        /*妈呀日期的减法怎么这么难搞只能都用最笨的办法来了*/

        for (int caldays = days; caldays >= 1; --caldays)
        {
            date1.day--; // 自减一天。

            if (date1.day == 0)
            {
                date1.month--;
                // 处理二月份，分平、闰年的情况

                if (IsLeapYear(date1.year)) // 闰年
                {
                    if (date1.month == 2)
                    {
                        date1.day = 29;
                    }
                    else if (date1.month == 1 || date1.month == 3 || date1.month == 5 || date1.month == 7 || date1.month == 8 || date1.month == 10 || date1.month == 12) // 大月
                    {
                        date1.day = 31;
                    }
                    else if (date1.month == 4 || date1.month == 6 || date1.month == 9 || date1.month == 11) // 小月
                    {
                        date1.day = 30;
                    }
                    else if (date1.month == 0)
                    {
                        date1.month = 12;
                        date1.day = 31;
                        date1.year--;
                    }
                }
                else // 平年
                {
                    if (date1.month == 2)
                    {
                        date1.day = 28;
                    }
                    else if (date1.month == 1 || date1.month == 3 || date1.month == 5 || date1.month == 7 || date1.month == 8 || date1.month == 10 || date1.month == 12) // 大月
                    {
                        date1.day = 31;
                    }
                    else if (date1.month == 4 || date1.month == 6 || date1.month == 9 || date1.month == 11) // 小月
                    {
                        date1.day = 30;
                    }
                    else if (date1.month == 0)
                    {
                        date1.month = 12;
                        date1.day = 31;
                        date1.year--;
                    }
                }
            }

        } // 把日期减到0估计就对了吧？

        /*----------------------------------------------------------------------------------------------*/

        return date1; // 减法的直接返回即可。
    }

    date1.day = days;

    return date1;
}

int CCnDays::GetDaysOfMonth(int year, int month)
{
    // 返回year年中month月的天数。

    if (IsLeapYear(year) && month == 1)
    {
        return DaysOfMonth[month] + 1;
    }

    return DaysOfMonth[month];
}

int *GetNow()
{
    time_t now;
    time(&now);
    tm *t = localtime(&now);
    int *res = (int *)malloc(3 * sizeof(int));
    res[0] = t->tm_year + 1900;
    res[1] = t->tm_mon + 1;
    res[2] = t->tm_mday;
    return res;
}

int main(int argc, char **argv)
{
    CCnDays *cndays = new CCnDays;
    int year = 0, month = 0, day = 0;
    if (argc == 4)
    {
        year = atoi(argv[1]);
        month = atoi(argv[2]);
        day = atoi(argv[3]);
    }
    else
    {
        int *today = GetNow();
        year = today[0];
        month = today[1];
        day = today[2];
    }
    structDate lunar = cndays->GetLunar(year, month, day);
    cout << lunar.cnYear << lunar.cnMonth << lunar.cnDay;
    delete cndays;
}
#include "stime.h"

RTC_HandleTypeDef* stime_hrtc = 0;

time_t stime(void)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(stime_hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(stime_hrtc, &sDate, RTC_FORMAT_BIN);

    struct tm t = {};

    t.tm_year = sDate.Year;
    t.tm_mon = sDate.Month;
    t.tm_mday = sDate.Date;
    t.tm_wday = sDate.WeekDay;

    t.tm_hour = sTime.Hours;
    t.tm_min = sTime.Minutes;
    t.tm_sec = sTime.Seconds;

    return mktime(&t);
}

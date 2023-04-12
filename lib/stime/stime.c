#include "stime.h"

#ifndef RTC_FORMAT_BIN
#define RTC_FORMAT_BIN 0

typedef int RTC_HandleTypeDef;
typedef struct { int Hours, Minutes, Seconds; } RTC_TimeTypeDef;
typedef struct { int Year, Month, Date, WeekDay; } RTC_DateTypeDef;
void HAL_RTC_GetTime(RTC_HandleTypeDef* hrtc, RTC_TimeTypeDef* time, int _)
{
    time->Hours = time->Minutes = time->Seconds = 0;
}
void HAL_RTC_GetDate(RTC_HandleTypeDef* hrtc, RTC_DateTypeDef* date, int _)
{
    date->Year = date->Month = date->Date = date->WeekDay = 0;
}

#endif /* RTC_FORMAT_BIN */


RTC_HandleTypeDef* stime_hrtc = 0;

time_t stime(void)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(stime_hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(stime_hrtc, &sDate, RTC_FORMAT_BIN);

    struct tm t = {};

    t.tm_hour = sTime.Hours;
    t.tm_min = sTime.Minutes;
    t.tm_sec = sTime.Seconds;

    t.tm_year = sDate.Year;
    t.tm_mon = sDate.Month;
    t.tm_mday = sDate.Date;
    t.tm_wday = sDate.WeekDay;

    return mktime(&t);
}

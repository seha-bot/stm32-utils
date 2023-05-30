#ifndef STM_UTILS_STIME
#define STM_UTILS_STIME

#include "main.h"
#include<time.h>

#ifdef RTC_FORMAT_BIN
extern RTC_HandleTypeDef* stime_hrtc;
#endif /* RTC_FORMAT_BIN */
time_t stime(void);
void stime_adjust_host(void);

#endif /* STM_UTILS_STIME */

#ifndef STM_UTILS_STIME
#define STM_UTILS_STIME

#include "main.h"
#include<time.h>

extern RTC_HandleTypeDef* stime_hrtc;
time_t get_time(void);

#endif /* STM_UTILS_STIME */

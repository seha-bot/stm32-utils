#ifndef STM_UTILS_SLEEP
#define STM_UTILS_SLEEP

#include "main.h"

#define sleep_ms(ms) HAL_Delay(ms)

void sleep_init(void);
void sleep_us(uint32_t us);

#endif /* STM_UTILS_SLEEP */

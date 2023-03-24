#ifndef STM_UTILS_IR
#define STM_UTILS_IR

#include "main.h"
#include "sleep.h"

/* TODO: make an init function for sleep */

#define IR GPIOA, GPIO_PIN_6

uint64_t ir_get(void);

#endif /* STM_UTILS_IR */

#ifndef STM_UTILS_IR
#define STM_UTILS_IR

#include "main.h"
#include "sleep.h"

/* TODO: make an init function and define IR in user code */

#define IR GPIOA, GPIO_PIN_6

uint64_t get_ir(void);

#endif /* STM_UTILS_IR */

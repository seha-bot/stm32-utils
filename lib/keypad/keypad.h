#ifndef STM_UTILS_KEYPAD
#define STM_UTILS_KEYPAD

#include "main.h"
#include "sleep.h"
#include<stdlib.h>
#include<stdarg.h>

typedef struct
{
    int width;
    int height;
    GPIO_TypeDef** ports;
    uint16_t* pins;
    int down;
} keypad;

/*
NOTE:
    The pins along the width (A0, A1 in the example) must be set to output
    and the pins along the height must be set to input (pull down)
EXAMPLE:
    keypad handler = keypad_init(2, 3,
        GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1,
        GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_4,
    );
*/
keypad keypad_init(int width, int height, ...);
int keypad_get(keypad* handle);

#endif /* STM_UTILS_KEYPAD */

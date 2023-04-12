#include "keypad.h"

keypad keypad_init(int width, int height, ...)
{
    keypad handle;
    handle.width = width;
    handle.height = height;
    int length = width + height;
    handle.ports = malloc(length * sizeof(void*));
    handle.pins = malloc(length * sizeof(uint16_t));

    va_list args;
    va_start(args, height);
    for(int i = 0; i < length; i++)
    {
        handle.ports[i] = va_arg(args, void*);
        handle.pins[i] = va_arg(args, int);
    }
    va_end(args);

    return handle;
}

int keypad_get(keypad* handle)
{
    GPIO_TypeDef** ports = handle->ports;
    uint16_t* pins = handle->pins;
    for(int i = 0; i < handle->width; i++)
    {
        HAL_GPIO_WritePin(ports[i], pins[i], 1);
        sleep_ms(0); // This might become slow on bigger keyboards
        for(int j = 0; j < handle->height; j++)
        {
            int offset = handle->width + j;
            if(!HAL_GPIO_ReadPin(ports[offset], pins[offset])) continue;
            handle->down = i + j * handle->width;
            HAL_GPIO_WritePin(ports[i], pins[i], 0);
            return 1;
        }
        HAL_GPIO_WritePin(ports[i], pins[i], 0);
    }
    return 0;
}

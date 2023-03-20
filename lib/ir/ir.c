#include "ir.h"

uint64_t get_ir(void)
{
    if(!HAL_GPIO_ReadPin(IR))
    {
        uint64_t fin = 0;
        uint8_t newValue, oldValue = 0, last = 0;
        for(int i = 0; i < 350; i++)
        {
            newValue = HAL_GPIO_ReadPin(IR);
            if(newValue != oldValue)
            {
                fin |= (i - last < 5);
                fin <<= 1;
                last = i;
            }
            oldValue = newValue;
            sleep_us(200);
        }
        return fin;
    }
    return 0;
}

#include "master.h"
#include<stdio.h>

keypad keypad_handler;
char buffer[16];
int blen = 0;
char map[12] = { '3','2','1','6','5','4','9','8','7',0,'0',0 };
int downs[12] = { [0 ... 11] = 0 };

void start(void)
{
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
    lcd_init();
    lcd_display_control(1, 1, 0);
    keypad_handler = keypad_init(3, 4,
        GPIOB, GPIO_PIN_12, GPIOB, GPIO_PIN_13, GPIOB, GPIO_PIN_14,
        GPIOB, GPIO_PIN_15, GPIOA, GPIO_PIN_8, GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_10
    );
}

uint16_t hash_32(uint32_t n)
{
    return (n & 0xffff) ^ ((n >> 16) & 0xffff);
}

uint16_t rand(uint32_t seed)
{
    return hash_32(hash_32(seed * 654) * hash_32(seed * 54323));
}

void loop(void)
{
    // time_t t = stime();
    // struct tm tmt = *localtime(&t);

    // sprintf(buffer, "%02d:%02d:%02d", tmt.tm_hour, tmt.tm_min, tmt.tm_sec);
    if(!keypad_get(&keypad_handler))
    {
        for(int i = 0; i < 12; i++) downs[i] = 0;
        return;
    }
    char key = map[keypad_handler.down];
    if(downs[keypad_handler.down]) return;
    downs[keypad_handler.down] = 1;

    if(keypad_handler.down == 11)
    {
        lcd_clear();
        blen = 0;
        return;
    }
    if(keypad_handler.down == 9)
    {
        lcd_letter('A');
        return;
    }

    if(blen == 15) return;
    buffer[blen++] = key;
    buffer[blen] = 0;

    lcd_set_cursor(0, 0);
    lcd_letters(buffer);
}

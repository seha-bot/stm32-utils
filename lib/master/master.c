#include "master.h"
#include<stdio.h>
#include<time.h>

keypad keypad_handler;
char buffer[16];
int blen = 0;
char map[12] = { '3','2','1','6','5','4','9','8','7',0,'0',0 };
int downs[12] = { [0 ... 11] = 0 };
uint64_t seed = 0;
time_t offset = 0;
time_t timer = 0;
int mode = 0;

void print_mode()
{
    if(mode != 2) lcd_clear();
    if(mode == 0) lcd_letters("INPUT SEED");
    else if(mode == 1) lcd_letters("INPUT PASSWORD");
    else if(mode == 2)
    {
        lcd_set_cursor(9, 0);
        lcd_letter(timer - stime() + '0');
    }
    lcd_set_cursor(0, 1);
    blen = 0;
}

void start(void)
{
    lcd_init();
    lcd_display_control(1, 1, 0);
    print_mode();

    keypad_handler = keypad_init(3, 4,
        GPIOB, GPIO_PIN_12, GPIOB, GPIO_PIN_13, GPIOB, GPIO_PIN_14,
        GPIOB, GPIO_PIN_15, GPIOA, GPIO_PIN_8, GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_10
    );
}

uint16_t hash_32(uint32_t n)
{
    return (n & 0xffff) ^ ((n >> 16) & 0xffff);
}

uint16_t hash_rand(uint32_t seed)
{
    return hash_32(hash_32(seed * 654) * hash_32(seed * 54323));
}

void loop(void)
{
    if(mode == 2)
    {
        if(timer - stime() <= 0)
        {
            lcd_display_control(1, 1, 0);
            mode = 1;
        }
        print_mode();
        sleep_ms(300);
        return;
    }

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
        print_mode();
        if(mode == 0) seed = 0;
        return;
    }
    if(keypad_handler.down == 9)
    {
        if(mode == 0)
        {
            offset = stime();
            mode = 1;
            print_mode();
            return;
        }
        uint16_t pass = hash_rand((stime() - offset + seed) / 60);
        if((pass & 0x000f) > 0x0009) pass &= 0xfff9;
        if((pass & 0x00f0) > 0x0090) pass &= 0xff9f;
        if((pass & 0x0f00) > 0x0900) pass &= 0xf9ff;
        if((pass & 0xf000) > 0x9000) pass &= 0x9fff;

        lcd_set_cursor(0, 1);

        uint16_t input = 0;
        for(int i = 0; i < 4; i++)
        {
            input <<= 4;
            input |= buffer[i] - '0';
        }

        if(blen != 4 || pass != input)
        {
            lcd_letters("BAD PASSWORD");
            sleep_ms(1000);
        }
        else
        {
            mode = 2;
            timer = stime() + 9;
            lcd_clear();
            lcd_display_control(1, 0, 0);
            lcd_letters("UNLOCKED ");
        }
        print_mode();
        return;
    }
    
    if(mode == 0)
    {
        seed *= 10;
        seed += key - '0';
    }

    if(blen == 15) return;
    buffer[blen++] = key;
    buffer[blen] = 0;
    lcd_set_cursor(0, 1);
    lcd_letters(buffer);
}

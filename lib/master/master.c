// This is the worst code I've written in my life
#include "master.h"
#include "rtc.h"
#include <stdio.h>
#include <time.h>

#define KEY_CLEAR 11
#define KEY_OK 9
#define MODE_SEED 0
#define MODE_LOCKED 1
#define MODE_UNLOCKED 2

keypad keypad_handler;
char buffer[16];
int blen = 0;
char map[12] = { '3','2','1','6','5','4','9','8','7',0,'0',0 };
int is_down[12] = { [0 ... 11] = 0 };
uint64_t seed = 0;
time_t timer = 0;
int mode = MODE_SEED;

// TODO: extract to flash-utils and make a proper manager.
#define FLASH_SEED_ADDR ((uint32_t)0x08007FF0)

// Warning: can corrupt all data :)
void unsafeEraseLastSector()
{
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInitStruct;
    eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.PageAddress = FLASH_SEED_ADDR;
    eraseInitStruct.NbPages = 1;
    uint32_t pageError = 0;
    HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
    HAL_FLASH_Lock();
}

void writeDataToFlash(uint32_t data, uint32_t addr)
{
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data);
    HAL_FLASH_Lock();
}

void writeSeed()
{
    writeDataToFlash(seed, FLASH_SEED_ADDR);
}

uint32_t readSeed()
{
    return *(uint32_t*)FLASH_SEED_ADDR;
}

void conditionalReadIfSet()
{
    if(readSeed() == 0xFFFFFFFF) return;
    seed = readSeed();
    mode = MODE_LOCKED;
}

void print_mode(void)
{
    lcd_clear();
    switch(mode)
    {
    case MODE_SEED: lcd_letters("INPUT SEED"); break;
    case MODE_LOCKED: lcd_letters("INPUT PASSWORD"); break;
    case MODE_UNLOCKED:
        lcd_letters("UNLOCKED ");
        lcd_letter(timer - stime() + '0');
        break;
    }
    lcd_set_cursor(0, 1);
    blen = 0;
}

void buzz(int ms)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);
    sleep_ms(ms);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
}

void lock(void)
{
    mode = MODE_LOCKED;
    lcd_display_control(1, 1, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
}

void unlock(void)
{
    mode = MODE_UNLOCKED;
    lcd_display_control(1, 0, 0);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    buzz(200);
    lcd_clear();
    timer = stime() + 9;
}

uint16_t hash_32(uint32_t n)
{
    return (n & 0xffff) ^ ((n >> 16) & 0xffff);
}

uint16_t hash_rand(uint32_t seed)
{
    return hash_32(hash_32(seed * 654) * hash_32(seed * 54323));
}

uint16_t get_pass(void)
{
    uint16_t pass = hash_rand((stime() + seed) / 60);
    for(uint8_t i = 0; i <= 12; i += 4)
    {
        pass = (pass & ~(0xf << i)) | ((pass >> i & 0xf) % 10 << i);
    }
    return pass;
}

void start(void)
{
    lcd_init();
    lcd_display_control(1, 1, 0);
    conditionalReadIfSet();
    print_mode();

    HAL_RTC_MspInit(&hrtc);

    keypad_handler = keypad_init(3, 4,
        GPIOB, GPIO_PIN_12, GPIOB, GPIO_PIN_13, GPIOB, GPIO_PIN_14,
        GPIOB, GPIO_PIN_15, GPIOA, GPIO_PIN_8, GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_10
    );
}

void loop(void)
{
    if(mode == MODE_UNLOCKED)
    {
        if(timer - stime() <= 0) lock();
        print_mode();
        sleep_ms(900);
        return;
    }

    if(!keypad_get(&keypad_handler))
    {
        for(int i = 0; i < 12; i++) is_down[i] = 0;
        return;
    }
    if(is_down[keypad_handler.down]) return;
    buzz(50);
    char key = map[keypad_handler.down];
    is_down[keypad_handler.down] = 1;

    switch(keypad_handler.down)
    {
    case KEY_CLEAR:
        print_mode();
        if(mode == MODE_SEED) seed = 0;
        return;
    case KEY_OK:
        if(mode == MODE_SEED)
        {
            mode = MODE_LOCKED;
            stime_adjust_host();
            seed = atoi(buffer);
            unsafeEraseLastSector();
            writeSeed();
            print_mode();
            return;
        }

        uint16_t input = 0;
        for(int i = 0; i < 4; i++)
        {
            input <<= 4;
            input |= buffer[i] - '0';
        }

        if(blen != 4 || input != get_pass())
        {
            print_mode();
            lcd_letters("BAD PASSWORD");
            buzz(600);
            sleep_ms(400);
            print_mode();
        }
        else unlock();
        return;
    }
    
    if(blen == 15) return;
    buffer[blen++] = key;
    buffer[blen] = 0;
    lcd_letter(key);
}

#include "lcd.h"

// DISPLAY INSTRUCTIONS
void lcd_set_data(uint8_t RS, uint8_t data)
{
    HAL_GPIO_WritePin(LCD_RS, RS);
    HAL_GPIO_WritePin(LCD_D7, data>>3&1);
    HAL_GPIO_WritePin(LCD_D6, data>>2&1);
    HAL_GPIO_WritePin(LCD_D5, data>>1&1);
    HAL_GPIO_WritePin(LCD_D4, data>>0&1);
    HAL_GPIO_WritePin(LCD_E, 1);
    sleep_us(1);
    HAL_GPIO_WritePin(LCD_E, 0);
    sleep_us(50);
}

void lcd_instruction(uint8_t data)
{
    lcd_set_data(0, data>>4);
    lcd_set_data(0, data);
}

void lcd_init(void)
{
    sleep_init();
    sleep_ms(50);
    lcd_set_data(0, 0b0010); // Function set ( 4 bit mode )
    lcd_instruction(0b00101000); // Function set ( 4 bit mode, 2 lines, 5x8 )
    lcd_instruction(0b00000110); // Entry mode set ( increment, display shift off )
    lcd_display_control(1, 0, 0);
}

// DISPLAY BUILTIN FUNCTIONS
void lcd_display_control(uint8_t display, uint8_t cursor, uint8_t blinking)
{
    lcd_instruction(0b00001000 | display << 2 | cursor << 1 | blinking);
}

void lcd_set_cursor(uint8_t x, uint8_t y)
{
    lcd_instruction(0b10000000 | y << 6 | x);
}

void lcd_clear(void)
{
    lcd_instruction(0b00000001);
    sleep_ms(1);
}

void lcd_home(void)
{
    lcd_instruction(0b00000010);
    sleep_ms(1);
}

void lcd_shift(uint8_t direction)
{
    lcd_instruction(0b00010000 | direction << 3);
}

// DRAWING FUNCTIONS
void lcd_letter(char c)
{
    lcd_set_data(1, c>>4);
    lcd_set_data(1, c);
}

void lcd_letters(char* s)
{
    int i = 0;
    while(s[i] != 0) lcd_letter(s[i++]);
}

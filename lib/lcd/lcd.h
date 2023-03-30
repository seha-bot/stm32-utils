#ifndef STM_UTILS_LCD
#define STM_UTILS_LCD

/*
01 - GND
02 - VDD
03 - CONTRAST
04 - RS
05 - GND
06 - E
07 - *
08 - *
09 - *
10 - *
11 - D4
12 - D5
13 - D6
14 - D7
15 - V+
16 - V-
*/

#include "main.h"
#include "sleep.h"

#define LCD_RS GPIOA, GPIO_PIN_0
#define LCD_E  GPIOA, GPIO_PIN_1

#define LCD_D4 GPIOA, GPIO_PIN_2
#define LCD_D5 GPIOA, GPIO_PIN_3
#define LCD_D6 GPIOA, GPIO_PIN_4
#define LCD_D7 GPIOA, GPIO_PIN_5

// DISPLAY INSTRUCTIONS
void lcd_set_data(uint8_t RS, uint8_t data);
void lcd_instruction(uint8_t data);
void lcd_init(void);

// DISPLAY BUILTIN FUNCTIONS
void lcd_display_control(uint8_t display, uint8_t cursor, uint8_t blinking);
void lcd_set_cursor(uint8_t x, uint8_t y);
void lcd_clear(void);
void lcd_home(void);
void lcd_shift(uint8_t direction);

// DRAWING FUNCTIONS
void lcd_letter(char c);
void lcd_letters(char* s);

#endif /* STM_UTILS_LCD */

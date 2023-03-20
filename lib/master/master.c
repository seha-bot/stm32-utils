#include "master.h"
#include<stdio.h>

void start(void)
{
    lcd_init();
}

char buffer[16];

void loop(void)
{
    time_t t = get_time();
    struct tm tmt = *localtime(&t);

    sprintf(buffer, "%02d:%02d:%02d", tmt.tm_hour, tmt.tm_min, tmt.tm_sec);

    lcd_set_cursor(0, 0);
    lcd_letters(buffer);

    sleep_ms(500);
}

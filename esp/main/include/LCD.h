//
// Created by lukaszk on 16.07.2020.
//

#ifndef APP_TEMPLATE_LCD_H
#define APP_TEMPLATE_LCD_H

#include "driver/i2c.h"
#include "i2c-lcd1602.h"

class LCD {
public:
    LCD();
    void displayTemperature(int t1, int t2);
private:
    i2c_config_t connectionConfiguration = {};
    i2c_lcd1602_info_t *lcd_info = new i2c_lcd1602_info_t;

    static uint8_t _wait_for_user(void);
};


#endif //APP_TEMPLATE_LCD_H

//
// Created by lukaszk on 16.07.2020.
//

#ifndef APP_TEMPLATE_LCD_H
#define APP_TEMPLATE_LCD_H

#include "driver/i2c.h"

class LCD {
public:
    LCD();

private:
    i2c_config_t connectionConfiguration = {};
    static uint8_t _wait_for_user(void);
};


#endif //APP_TEMPLATE_LCD_H

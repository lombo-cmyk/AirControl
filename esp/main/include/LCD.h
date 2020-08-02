//
// Created by lukaszk on 16.07.2020.
//

#ifndef APP_TEMPLATE_LCD_H
#define APP_TEMPLATE_LCD_H

#include "driver/i2c.h"
#include "i2c-lcd1602.h"
#include <string>
#include "TemperatureSensor.h"
class LCD {
public:
    LCD();
    void displayTemperature(const float& outsideTemp,
                            const float& insideTemp) const;
    void displayScreen(std::array<float, MAX_DEVICES>& temp);

private:
    i2c_config_t connectionConfiguration = {};
    i2c_lcd1602_info_t* lcd_info = new i2c_lcd1602_info_t;
    std::string convertFloatToString(float number,
                                     std::uint8_t precision) const;
    void displayEnergyUsage(unsigned long int energy) const;
    void displayTime() const;
    void blank() const;
    void displayLine(const std::string& line, std::uint8_t row) const;
    static uint8_t _wait_for_user(void);
};

#endif // APP_TEMPLATE_LCD_H

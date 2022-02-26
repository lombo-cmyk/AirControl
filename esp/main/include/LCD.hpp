//
// Created by lukaszk on 16.07.2020.
//

#ifndef APP_TEMPLATE_LCD_H
#define APP_TEMPLATE_LCD_H

#include "driver/i2c.h"
#include "i2c-lcd1602.h"
#include <string>
#include "TemperatureSensor.hpp"

class LCD {
public:
    LCD();
    static void AdjustLine(std::string& line);
    void DisplayScreen(std::array<float, MAX_DEVICES>& temp);

private:
    i2c_config_t connectionConfiguration_ = {};
    i2c_lcd1602_info_t* LcdInfo_ = new i2c_lcd1602_info_t;
    bool isBacklight_ = true;
    std::uint64_t backlightTimer_ = esp_timer_get_time();

    void InitializeConnectionConfiguration();
    template<typename T>
    std::string ConvertNumberToString(T number, std::uint8_t precision) const;
    void DisplayLine(std::string& line, std::uint8_t row) const;
    void DisplayTwoLines(std::string& line_1, std::string& line_2) const;
    void DisplayTime() const;
    void DisplayTemperature(const float& outsideTemp,
                            const float& insideTemp) const;
    void DisplayEnergyUsage(const std::uint64_t& impulses) const;
    void DisplayAirSource() const;
    void DisplayWelcomeMessage() const;
    void Setbacklight(const std::uint16_t& displayState);
    std::string ProcessTime(const tm& t) const;
    std::string ProcessDate(const tm& t) const;
    std::tuple<std::string, std::string> GetDateTime() const;
    static uint8_t _wait_for_user(void);
};

#endif // APP_TEMPLATE_LCD_H
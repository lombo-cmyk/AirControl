//
// Created by lukaszk on 16.07.2020.
//

#ifndef APP_TEMPLATE_LCD_H
#define APP_TEMPLATE_LCD_H

#include <string>
#include "driver/i2c.h"
#include "i2c-lcd1602.h"
#include "LcdBase.hpp"
#include "ObserverBase.hpp"
#include "RomHashMap.hpp"
#include "TemperatureSensor.hpp"

class LCD : public Observer, protected LcdBase {
public:
    void Init(std::uint8_t numberOfColumns, std::uint8_t address) override;
    void DisplayWelcomeMessage() const override;
    void DisplayCurrentState() override;

private:
    static constexpr std::string_view LcdTag_ = "LCD";
    static constexpr std::string_view outsideKey_ = "outside";
    static constexpr std::string_view insideKey_ = "inside";

    bool isBacklight_ = true;
    bool isOverride_ = false;
    bool useInsideAirManual_ = false;
    std::uint64_t pumpEnergyUsage_ = 0;
    StringViewHashMap temperature_;
    std::uint8_t displayState_ = 0;

    void DisplayTime() const;
    void DisplayTemperature() const;
    void DisplayEnergyUsage() const;
    void DisplayAirSource() const;
    void Setbacklight() override;

    static std::string ProcessTime(const tm& t);
    static std::string ProcessDate(const tm& t);
    static std::tuple<std::string, std::string> GetDateTime();

    void update() override;
};

#endif // APP_TEMPLATE_LCD_H

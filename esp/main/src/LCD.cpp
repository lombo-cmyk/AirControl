//
// Created by lukaszk on 16.07.2020.
//

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Definitions.hpp"
#include "InterruptHandler.hpp"
#include "Logger.hpp"
#include "sdkconfig.h"
#include "smbus.h"

#include "LCD.hpp"

void LCD::Init(std::uint8_t numberOfColumns, std::uint8_t address) {
    LcdBase::Init(numberOfColumns, address);
    InterruptHandler::AttachObserver(this);
    TemperatureSensor::AttachObserver(this);
}

void LCD::DisplayWelcomeMessage() const {
    std::string welcomeMessage1 = "Welcome to";
    std::string welcomeMessage2 = "AirControl v1";
    try {
        DisplayTwoLines(welcomeMessage1, welcomeMessage2);
    } catch (const std::invalid_argument& e) {
        LogInfo(LcdTag_, "Lcd exception thrown: ", e.what());
    }
}

void LCD::DisplayCurrentState() {
    // Dependent on MAX_DISPLAY_STATE define
    Setbacklight();
    switch (displayState_) {
    case TimeScreen:
        LCD::DisplayTime();
        break;
    case TempScreen:
        LCD::DisplayTemperature();
        break;
    case EnergyScreen:
        LCD::DisplayEnergyUsage();
        break;
    case AirSourceScreen:
        LCD::DisplayAirSource();
        break;
    default:
        break;
    }
}

void LCD::DisplayTime() const {
    /*Network synchronized time is displayed here*/
    auto dateTime = GetDateTime();
    try {
        LCD::DisplayTwoLines(std::get<0>(dateTime), std::get<1>(dateTime));
    } catch (const std::invalid_argument& e) {
        LogInfo(LcdTag_, "Lcd exception thrown: ", e.what());
    }
}

void LCD::DisplayTemperature() const {
    std::uint8_t precision = 1;
    std::string firstLine = "Temp zewn: " +
                            ConvertNumberToString(
                                temperature_.find(outsideKey_)->second,
                                precision);
    std::string secondLine = "Temp wewn: " +
                             ConvertNumberToString(
                                 temperature_.find(insideKey_)->second,
                                 precision);
    try {
        DisplayTwoLines(firstLine, secondLine);
    } catch (const std::invalid_argument& e) {
        LogInfo(LcdTag_, "Lcd exception thrown: ", e.what());
    }
}

void LCD::DisplayEnergyUsage() const {
    const std::size_t impulsesPerKwh = 6400;
    double usedEnergy = static_cast<double>(pumpEnergyUsage_) / impulsesPerKwh;
    std::string s1 = "Energy usage:";
    std::string s2 = ConvertNumberToString(usedEnergy, 3) + " kWh";
    try {
        LCD::DisplayTwoLines(s1, s2);
    } catch (const std::invalid_argument& e) {
        LogInfo(LcdTag_, "Lcd exception thrown: ", e.what());
    }
}

void LCD::DisplayAirSource() const {
    /*State of Air control will be displayed here. This screen will also allow
     * user to override default valve state*/
    std::string line_1 = "Tryb: ";
    line_1 += isOverride_ ? "Manual" : "Automat";
    std::string line_2 = "Zrodlo: ";
    if (!isOverride_) {
        line_2 += "Automat";
    } else {
        line_2 += useInsideAirManual_ ? "Wewn" : "Zewn";
    }
    try {
        LCD::DisplayTwoLines(line_1, line_2);
    } catch (const std::invalid_argument& e) {
        LogInfo(LcdTag_, "Lcd exception thrown: ", e.what());
    }
}

void LCD::Setbacklight() {
    uint64_t tenSeconds = SECOND * 10;
    if (displayState_ == 0 && isBacklight_) {
        i2c_lcd1602_set_backlight(&LcdInfo_, true);
        backlightTimer_ = esp_timer_get_time();
        InterruptHandler::SetBacklightFromLcd() = false;
    }
    esp_timer_get_time();
    if (displayState_ == 0 &&
        (esp_timer_get_time() - backlightTimer_) > tenSeconds) {
        i2c_lcd1602_set_backlight(&LcdInfo_, false);
    }
    if (displayState_ != 0) {
        backlightTimer_ = esp_timer_get_time();
        i2c_lcd1602_set_backlight(&LcdInfo_, true);
    }
}

std::string LCD::ProcessTime(const tm& t) {
    std::stringstream buffer;
    buffer << std::setfill('0') << std::setw(2) << t.tm_hour << ":"
           << std::setfill('0') << std::setw(2) << t.tm_min << ":"
           << std::setfill('0') << std::setw(2) << t.tm_sec;
    return buffer.str();
}

std::string LCD::ProcessDate(const tm& t) {
    std::stringstream buffer;
    buffer << std::setfill('0') << std::setw(2) << t.tm_mday << "."
           << std::setfill('0') << std::setw(2) << t.tm_mon + 1 << "."
           << t.tm_year + 1900;
    return buffer.str();
}
std::tuple<std::string, std::string> LCD::GetDateTime() {
    auto timeNow = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(timeNow);
    tm local_tm = *localtime(&tt);
    return {ProcessDate(local_tm), ProcessTime(local_tm)};
}

void LCD::update() {
    displayState_ = InterruptHandler::GetDisplayState();
    isBacklight_ = InterruptHandler::GetLcdBacklight();
    isOverride_ = InterruptHandler::GetOverride();
    useInsideAirManual_ = InterruptHandler::GetManualInfo();
    pumpEnergyUsage_ = InterruptHandler::GetPumpEnergyUsage();
    auto& t = TemperatureSensor::getInstance();
    temperature_ = t.GetTemperature();
}

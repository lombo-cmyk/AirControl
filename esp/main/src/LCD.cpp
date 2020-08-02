//
// Created by lukaszk on 16.07.2020.
//

#include "LCD.h"
#include "sdkconfig.h"
#include "smbus.h"
#include "Definitions.h"
#include "string"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "ElectricMeter.h"
LCD::LCD() {
    connectionConfiguration.mode = I2C_MODE_MASTER;
    connectionConfiguration.sda_io_num = LCD_SDA_PIN;
    connectionConfiguration.scl_io_num = LCD_SCL_PIN;
    connectionConfiguration.sda_pullup_en = GPIO_PULLUP_ENABLE;
    connectionConfiguration.scl_pullup_en = GPIO_PULLUP_ENABLE;
    connectionConfiguration.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &connectionConfiguration);
    i2c_driver_install(I2C_NUM_0, connectionConfiguration.mode, 0, 0, 0);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_port_t i2c_num = I2C_NUM_0;
    uint8_t address = 0x27;
    smbus_info_t* smbus_info = smbus_malloc();
    ESP_ERROR_CHECK(smbus_init(smbus_info, i2c_num, address));
    ESP_ERROR_CHECK(smbus_set_timeout(smbus_info, 1000 / portTICK_RATE_MS));
    ESP_ERROR_CHECK(i2c_lcd1602_init(lcd_info, smbus_info, true, 2, 32, 16));

    ESP_ERROR_CHECK(i2c_lcd1602_reset(lcd_info));
    ESP_ERROR_CHECK(i2c_lcd1602_reset(lcd_info));
    _wait_for_user();
    i2c_lcd1602_set_backlight(lcd_info, true);
    _wait_for_user();
    std::string message = "Short message";
    uint8_t pos = 0;
    for (const char& letter : message) {
        i2c_lcd1602_move_cursor(lcd_info, pos++, 0);
        i2c_lcd1602_write_char(lcd_info, letter);
    }
}

void LCD::displayLine(const std::string& line, std::uint8_t row) const {
    std::uint8_t pos = 0;
    if (line.length() > 16) {
        std::string lcd_err = "Line too long!";
        for (const char& letter : lcd_err) {
            i2c_lcd1602_move_cursor(lcd_info, pos++, row);
            i2c_lcd1602_write_char(lcd_info, letter);
        }
        throw std::invalid_argument("Line too long!");
    }
    for (const char& letter : line) {
        i2c_lcd1602_move_cursor(lcd_info, pos++, row);
        i2c_lcd1602_write_char(lcd_info, letter);
    }
}

std::string LCD::convertFloatToString(float number,
                                      std::uint8_t precision) const {
    std::stringstream stringStream;
    stringStream << std::fixed << std::setprecision(precision) << number;
    return stringStream.str();
}

void LCD::displayTemperature(const float& outsideTemp,
                             const float& insideTemp) const {
    std::uint8_t precision = 1;
    i2c_lcd1602_clear(lcd_info);
    std::string row0 = "Temp zewn: " +
                       convertFloatToString(outsideTemp, precision);
    std::string row1 = "Temp wewn: " +
                       convertFloatToString(insideTemp, precision);
    try {
        displayLine(row0, 0);
        displayLine(row1, 1);
    } catch (const std::invalid_argument& e) {
        std::cout << "Lcd exception thrown: " << e.what() << std::endl;
    }
}

uint8_t LCD::_wait_for_user(void) {
    uint8_t c = 0;

#ifdef USE_STDIN
    while (!c) {
        STATUS s = uart_rx_one_char(&c);
        if (s == OK) {
            printf("%c", c);
        }
        vTaskDelay(1);
    }
#else
    vTaskDelay(1000 / portTICK_RATE_MS);
#endif
    return c;
}

void LCD::displayScreen(std::array<float, MAX_DEVICES>& temp) {
    switch (ElectricMeter::GetDisplayState()) {
    case 0:
        LCD::displayTime();
        break;
    case 1:
        LCD::displayTemperature(temp[0], temp[1]);
        break;
    case 2:
        LCD::displayEnergyUsage(ElectricMeter::GetPumpEnergyUsage());
        break;
    case 3:
        LCD::blank();
        break;
    default:
        break;
    }
}

void LCD::displayEnergyUsage(unsigned long int energy) const {
    i2c_lcd1602_clear(lcd_info);
    std::string s2 = "Energy usage:" + std::to_string(energy);
    LCD::displayLine(s2, 0);
}

void LCD::displayTime() const {
    i2c_lcd1602_clear(lcd_info);
    std::string s1 = "Time here 1";
    std::string s2 = "Time here 2";
    LCD::displayLine(s1, 0);
    LCD::displayLine(s2, 1);
}

void LCD::blank() const {
    i2c_lcd1602_clear(lcd_info);
    LCD::displayLine("blank line", 0);
}

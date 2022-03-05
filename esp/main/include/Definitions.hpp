//
// Created by lukaszk on 08.07.2020.
//
#include "driver/gpio.h"

#ifndef AIRCONTROLLER_PINS_H
#define AIRCONTROLLER_PINS_H

#define TEMPERATURE_PIN GPIO_NUM_4
#define ELECTRIC_METER_PIN_1 GPIO_NUM_23
#define FORWARD_BUTTON GPIO_NUM_5
#define OVERRIDE_BUTTON GPIO_NUM_19
#define AIR_SOURCE_BUTTON GPIO_NUM_18
#define LCD_SDA_PIN GPIO_NUM_21
#define LCD_SCL_PIN GPIO_NUM_22
#define AIR_SOURCE_OUTPUT GPIO_NUM_12
#define LCD_BACKLIGHT_PIN GPIO_NUM_18

#define ESP_INTR_FLAG_DEFAULT 0
#define MAX_DEVICES 2
#define DS18B20_USED_RESOLUTION (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD 10000 // milliseconds
#define DEBOUNCE_TIME 70
#define LCD_COLUMNS 16
#define SECOND 1000
#endif // AIRCONTROLLER_PINS_H

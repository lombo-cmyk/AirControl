//
// Created by lukaszk on 08.07.2020.
//
#include "driver/gpio.h"

#ifndef AIRCONTROLLER_PINS_H
#define AIRCONTROLLER_PINS_H

#define temperaturePin GPIO_NUM_2
#define electricMeterPin_1 GPIO_NUM_4
#define electricMeterPin_2 GPIO_NUM_5
#define insideAirUsagePin GPIO_NUM_12
#define outsideAirUsagePin GPIO_NUM_13
#define forwardButtonPin GPIO_NUM_14
#define backwardButtonPin GPIO_NUM_26
#define optionsButtonPin GPIO_NUM_27
#define lcdSdaPin GPIO_NUM_21
#define lcdSclPin GPIO_NUM_22
#define lcdMasterPin GPIO_NUM_5

#endif //AIRCONTROLLER_PINS_H

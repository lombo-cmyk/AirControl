//
// Created by lukaszk on 06.07.2020.
//
#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <PinHandling.h>
#include "Pins.h"
#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"
#include <memory>

#define MAX_DEVICES          2
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD        (1000)   // milliseconds

PinHandling::PinHandling() {
//    gpio_set_direction(temperaturePin, GPIO_MODE_OUTPUT);
//    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
//    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_INPUT);
//    gpio_set_intr_type(GPIO_NUM_23, GPIO_INTR_POSEDGE);

}


void PinHandling::ChangeStateOnImpuls(void *state) {
    bool tmp = static_cast<bool>(*(bool *)state);
    gpio_set_level(GPIO_NUM_32, tmp);
    tmp=!tmp;
    *(bool*)state = tmp;
}

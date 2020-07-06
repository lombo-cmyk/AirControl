//
// Created by lukaszk on 06.07.2020.
//
#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <PinHandling.h>

PinHandling::PinHandling() {
    gpio_pad_select_gpio(GPIO_NUM_33);
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_INPUT);
    gpio_set_intr_type(GPIO_NUM_23, GPIO_INTR_POSEDGE);
}

void PinHandling::Run() {
    for(;;) {
        printf("Turning off the LED\n");
        gpio_set_level(GPIO_NUM_33, 0);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        printf("Turning on the LED\n");
        gpio_set_level(GPIO_NUM_33, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void PinHandling::ChangeStateOnImpuls(void *state) {
    bool tmp = static_cast<bool>(*(bool *)state);
    gpio_set_level(GPIO_NUM_32, tmp);
    tmp=!tmp;
    *(bool*)state = tmp;
}

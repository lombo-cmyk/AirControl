//
// Created by lukaszk on 12.07.2020.
//

#ifndef AIRCONTROLLER_ELECTRICMETER_H
#define AIRCONTROLLER_ELECTRICMETER_H

#include "Pins.h"
#include "freertos/FreeRTOS.h"
#include "freertos//task.h"
#define ESP_INTR_FLAG_DEFAULT 0

struct ElectricMeter {
public:
    static void Start();
    static unsigned long int GetPumpEnergyUsage();

private:
    static unsigned long int ElectricMeterPump;
    static TickType_t lastWakePump;
    static void AddPumpEnergyUsage(void *arg);
};


#endif //AIRCONTROLLER_ELECTRICMETER_H

//
// Created by lukaszk on 12.07.2020.
//

#ifndef AIRCONTROLLER_ELECTRICMETER_H
#define AIRCONTROLLER_ELECTRICMETER_H

#include "Definitions.h"
#include "freertos//task.h"
#include "freertos/FreeRTOS.h"
#include <cstdint>

struct ElectricMeter {
public:
    static void Start();
    static std::uint64_t GetPumpEnergyUsage();

private:
    static unsigned long int ElectricMeterPump;
    static TickType_t lastWakePump;
    static void AddPumpEnergyUsage(void* arg);
};

#endif // AIRCONTROLLER_ELECTRICMETER_H

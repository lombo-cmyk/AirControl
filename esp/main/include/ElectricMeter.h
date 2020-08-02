//
// Created by lukaszk on 12.07.2020.
//

#ifndef AIRCONTROLLER_ELECTRICMETER_H
#define AIRCONTROLLER_ELECTRICMETER_H

#include "Definitions.h"
#include "freertos/FreeRTOS.h"
#include "freertos//task.h"
#include <cstdint>

struct ElectricMeter {
public:
    static void Start();
    static std::uint64_t GetPumpEnergyUsage();
    static std::uint16_t GetDisplayState();

private:
    static unsigned long int _ElectricMeterPump;
    static std::uint16_t _displayState;
    static TickType_t _lastWakeTimePump;
    static TickType_t _lastWakeTimeForwardButton;
    static void AddPumpEnergyUsage(void* arg);
    static void DisplayNextState(void* arg);
};

#endif // AIRCONTROLLER_ELECTRICMETER_H

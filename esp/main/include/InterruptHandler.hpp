//
// Created by lukaszk on 12.07.2020.
//

#ifndef AIRCONTROLLER_INTERRUPTHANDLER_HPP
#define AIRCONTROLLER_INTERRUPTHANDLER_HPP

#include "Definitions.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos//task.h"
#include <cstdint>

struct InterruptHandler {
public:
    static void Start();
    static std::uint64_t GetPumpEnergyUsage();
    static std::uint16_t GetDisplayState();
    static auto SetBacklightFromLcd() -> bool& {
        return lcdBacklight_;
    }
    static auto GetLcdBacklight() -> const bool& {
        return lcdBacklight_;
    }
    static auto GetOverride() -> const bool& {
        return override_;
    }
    static auto GetManualInfo() -> const bool& {
        return useInsideAirManual_;
    }

private:
    static std::uint64_t ElectricMeterPump_;
    static std::uint16_t displayState_;
    static bool lcdBacklight_;
    static bool override_;
    static bool useInsideAirManual_;
    static TickType_t lastWakeTimePump_;
    static TickType_t lastWakeTimeForwardButton_;
    static TickType_t lastWakeTimeOverrideButton_;
    static TickType_t lastWakeTimeInsideAirManualButton_;

    static void AddPumpEnergyUsage(void* arg);
    static void DisplayNextState(void* arg);
    static void OverrideAutomatic(void* arg);
    static void SetAirManually(void* arg);
};

#endif // AIRCONTROLLER_INTERRUPTHANDLER_HPP

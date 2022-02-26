//
// Created by lukaszk on 12.08.2020.
//

#ifndef AIRCONTROLLER_AIRCONTROLMOTOR_HPP
#define AIRCONTROLLER_AIRCONTROLMOTOR_HPP

#include <array>
#include "Definitions.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos//task.h"

class AirControlMotor {
public:
    void SetMotor(const std::array<float, MAX_DEVICES> temp,
                  const bool isOverride,
                  const bool isInsideAir);

private:
    double outsideTemperature_ = 0;
    double insideTemperature_ = 1;
    bool insideAirSource_ = true;
    TickType_t lastSourceChange_ = 0;
    void SetAirSource(const bool isOverride, const bool isInsideAir);
    void SetPin();
};

#endif // AIRCONTROLLER_AIRCONTROLMOTOR_HPP

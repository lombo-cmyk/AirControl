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
    void SetMotor(std::array<float, MAX_DEVICES> temp,
                  bool isOverride,
                  bool isInsideAir);

private:
    float outsideTemperature_ = 0;
    float insideTemperature_ = 1;
    bool insideAirSource_ = true;
    TickType_t lastSourceChange_ = 0;
    void SetAirSource(bool isOverride, bool isInsideAir);
    void SetPin();
};

#endif // AIRCONTROLLER_AIRCONTROLMOTOR_HPP

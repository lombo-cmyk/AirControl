//
// Created by lukaszk on 12.08.2020.
//

#include "../include/AirControlMotor.hpp"

void AirControlMotor::SetMotor(const std::array<float, MAX_DEVICES> temp,
                               const bool isOverride,
                               const bool isInsideAir) {
    outsideTemperature_ = temp[0];
    insideTemperature_ = temp[1];
    SetAirSource(isOverride, isInsideAir);
    SetPin();
}

void AirControlMotor::SetAirSource(const bool isOverride,
                                   const bool isInsideAir) {
    if (isOverride) {
        insideAirSource_ = isInsideAir != 0;
    } else {
        if (xTaskGetTickCount() - lastSourceChange_ > 120 * SECOND) {
            lastSourceChange_ = xTaskGetTickCount();
            if (insideTemperature_ - outsideTemperature_ > 1) {
                insideAirSource_ = true;
            } else if (outsideTemperature_ - insideTemperature_ > 1) {
                insideAirSource_ = false;
            }
        }
    }
}

void AirControlMotor::SetPin() {
    gpio_set_level(AIR_SOURCE_OUTPUT, !insideAirSource_);
}

//
// Created by lukaszk on 12.08.2020.
//

#include "AirControlMotor.hpp"
#include "InterruptHandler.hpp"
#include "Logger.hpp"
#include "TemperatureSensor.hpp"

void AirControlMotor::Init() {
    InterruptHandler::AttachObserver(this);
    TemperatureSensor::AttachObserver(this);
}

void AirControlMotor::update() {
    auto& t = TemperatureSensor::getInstance();
    std::unordered_map<std::string_view, float>
        temperature_ = t.GetTemperature();
    SetAirSource(InterruptHandler::GetOverride(),
                 InterruptHandler::GetManualInfo(),
                 temperature_);
    SetPin();
}

void AirControlMotor::SetAirSource(const bool isOverride,
                                   const bool isInsideAir,
                                   const StringViewHashMap& temperature) {
    if (isOverride) {
        insideAirSource_ = isInsideAir != 0;
    } else {
        if (xTaskGetTickCount() - lastSourceChange_ > 120 * SECOND) {
            UpdateAutomaticValues(temperature);
        }
    }
}
void AirControlMotor::SetPin() {
    gpio_set_level(AIR_SOURCE_OUTPUT, !insideAirSource_);
}
void AirControlMotor::UpdateAutomaticValues(
    const StringViewHashMap& temperature) {
    lastSourceChange_ = xTaskGetTickCount();
    bool tempInsideHigher = (temperature.find(insideKey_)->second -
                             temperature.find(outsideKey_)->second) >
                            MINIMUM_TEMP_THRESHOLD;
    bool tempInsideLower = (temperature.find(outsideKey_)->second -
                            temperature.find(insideKey_)->second) >
                           MINIMUM_TEMP_THRESHOLD;

    if (tempInsideHigher) {
        LogInfo(airControlTag_, "Changing air source - Inside air");
        insideAirSource_ = true;
    } else if (tempInsideLower) {
        insideAirSource_ = false;
        LogInfo(airControlTag_, "Changing air source - Outside air");
    }
}

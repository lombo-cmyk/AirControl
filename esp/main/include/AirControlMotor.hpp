//
// Created by lukaszk on 12.08.2020.
//

#ifndef AIRCONTROLLER_AIRCONTROLMOTOR_HPP
#define AIRCONTROLLER_AIRCONTROLMOTOR_HPP

#include <array>
#include <unordered_map>

#include "freertos/FreeRTOS.h"
#include "freertos//task.h"

#include "Definitions.hpp"
#include "ObserverBase.hpp"
#include "RomHashMap.hpp"
#include "Singleton.h"

class AirControlMotor final : Singleton<AirControlMotor>, Observer {
public:
    friend Singleton<AirControlMotor>;
    void Init();
    void update() override;

private:
    static constexpr std::string_view airControlTag_ = "Air Control";
    // move these to some common space one day
    static constexpr std::string_view outsideKey_ = "outside";
    static constexpr std::string_view insideKey_ = "inside";
    bool insideAirSource_ = true;
    TickType_t lastSourceChange_ = 0;
    void SetAirSource(bool isOverride,
                      bool isInsideAir,
                      const StringViewHashMap& temperature);
    void UpdateAutomaticValues(const StringViewHashMap& temperature);
    void SetPin();
};

#endif // AIRCONTROLLER_AIRCONTROLMOTOR_HPP

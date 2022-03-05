//
// Created by Lukasz on 01.03.2022.
//

#ifndef AIRCONTROLLER_STUBS_H
#define AIRCONTROLLER_STUBS_H

#include "Definitions.hpp"
#include "ObserverBase.hpp"
#include "InterruptHandler.hpp"
#include "Logger.hpp"

#ifdef CONFIG_STUB_LCD
class LCD: public Observer {
public:
    LCD(){
    InterruptHandler::AttachObserver(this);
    }
    static void AdjustLine(std::string& line){};
    void DisplayScreen(std::array<float, MAX_DEVICES>& temp){
        LogInfo("Stub LCD: ", "subbed value: ", subscribedValue);
    };
    void update() override{
        subscribedValue = InterruptHandler::GetDisplayState();
    };

private:
    std::uint16_t subscribedValue = 0;

};
#else
#include "LCD.hpp"
#endif

#ifdef CONFIG_STUB_TEMP
class TemperatureSensor final : public Singleton<TemperatureSensor> {
public:
    friend Singleton<TemperatureSensor>;
    void Init(){};
    void Run(){};
};
#else
#include "TemperatureSensor.hpp"
#endif

#endif // AIRCONTROLLER_STUBS_H

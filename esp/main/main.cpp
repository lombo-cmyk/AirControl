#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "esp_log.h"
#include "InterruptHandler.hpp"
#include "Wifi.hpp"
#include "Logger.hpp"
#include "AirControlMotor.hpp"

#ifdef CONFIG_INCLUDE_STUBS
#include "Stubs.hpp"
#else
#include "LCD.hpp"
#include "TemperatureSensor.hpp"
#endif

extern "C" {
void app_main();
}

void app_main(void) {
    LogInfo("main", "Esp starting");
    TemperatureSensor temp = TemperatureSensor();
    InterruptHandler::Start();
    temp.FindDevices();
    temp.InitializeDevices();
    LCD LCDisplay = LCD();
    Wifi::StartWifi();
    std::array<float, MAX_DEVICES> temperature = {};
    AirControlMotor Motor;
    for (;;) {
        temperature = temp.PerformTemperatureReadOut();
        for (auto const& t : temperature) {
            std::cout << "Temp is: " << t << " C" << std::endl;
        }
        LCDisplay.DisplayScreen(temperature);
        LogInfo("main", "kWh Pump: ", InterruptHandler::GetPumpEnergyUsage());
        LogInfo("main", "LCD State: ", InterruptHandler::GetDisplayState());
        Motor.SetMotor(temperature,
                       InterruptHandler::GetOverride(),
                       InterruptHandler::GetManualInfo());

        std::time_t t = std::time(nullptr);
        std::ostringstream stream;
        stream << std::put_time(std::localtime(&t), "%d-%m-%Y %H:%M:%S");
        LogInfo("main", "The current date/time is: ", stream.str());
        vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    }
}

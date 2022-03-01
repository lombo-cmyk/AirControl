#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "esp_log.h"
#include "Logger.hpp"

#include "InterruptHandler.hpp"
#include "LCD.hpp"
#include "TemperatureSensor.hpp"
#include <AirControlMotor.hpp>
#include "Wifi.hpp"

extern "C" {
void app_main();
}

void app_main(void) {
    std::cout << "Esp starting" << std::endl;
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
        std::cout << "kWh Pump: " << InterruptHandler::GetPumpEnergyUsage()
                  << std::endl;
        std::cout << "LCD State: " << InterruptHandler::GetDisplayState()
                  << std::endl;
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

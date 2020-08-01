#include "ElectricMeter.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "include/TemperatureSensor.h"
#include <iostream>
#include <memory>

extern "C" {
void app_main();
}

void app_main(void) {
    std::cout << "Esp starting" << std::endl;
    TemperatureSensor temp = TemperatureSensor();
    ElectricMeter::Start();
    temp.FindDevices();
    temp.InitializeDevices();
    std::array<float, MAX_DEVICES> temperature = {};
    for (;;) {
        temperature = temp.PerformTemperatureReadOut();
        for (auto const& t : temperature) {
            std::cout << "Temp is: " << t << " C" << std::endl;
        }
        std::cout << "kWh Pump: " << ElectricMeter::GetPumpEnergyUsage()
                  << std::endl;

        vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    }
}

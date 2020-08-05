#include "ElectricMeter.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "LCD.h"
#include "include/TemperatureSensor.h"
#include <iostream>
#include <memory>
#include "src/Wifi.cpp"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
extern "C" {
void app_main();
}

void app_main(void) {
    std::cout << "Esp starting" << std::endl;
    TemperatureSensor temp = TemperatureSensor();
    ElectricMeter::Start();
    temp.FindDevices();
    temp.InitializeDevices();
    LCD LCDisplay = LCD();
    aaa();
    std::array<float, MAX_DEVICES> temperature = {};
    for (;;) {
        temperature = temp.PerformTemperatureReadOut();
        for (auto const& t : temperature) {
            std::cout << "Temp is: " << t << " C" << std::endl;
        }
        LCDisplay.DisplayScreen(temperature);
        std::cout << "kWh Pump: " << ElectricMeter::GetPumpEnergyUsage()
                  << std::endl;
        std::cout << "LCD State: " << ElectricMeter::GetDisplayState()
                  << std::endl;
        time_t now;
        char strftime_buf[64];
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(TAG, "The current date/time is: %s", strftime_buf);
        vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    }
}

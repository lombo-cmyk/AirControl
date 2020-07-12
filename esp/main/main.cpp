#include "driver/gpio.h"
#include <iostream>
#include <memory>
#include "include/TemperatureSensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ElectricMeter.h"
#define MAX_DEVICES          2
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD        (1000)   // milliseconds
#define ESP_INTR_FLAG_DEFAULT 0
extern "C"{
    void app_main();
}

void app_main(void){
    std::cout << "Esp starting" << std::endl;
    TemperatureSensor temp = TemperatureSensor();
    ElectricMeter::Start();
    temp.FindDevices();
    temp.InitializeDevices();
    std::array<float, MAX_DEVICES> temperature={};
    while(true){
        try{
            temperature = temp.PerformTemperatureReadOut();
        }
        catch (const std::exception& e){
            std::cout << "Temp readout failed with: " << e.what() << std::endl;
            esp_restart();
        }
        for (auto const& t : temperature){
            std::cout << "Temp is: " << t << " C" <<std::endl;
        }
        std::cout << "kWh Pump: " << ElectricMeter::GetPumpEnergyUsage() << std::endl;

        vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    }
}

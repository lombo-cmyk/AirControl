#include <stdio.h>
#include "driver/gpio.h"
#include <iostream>
#include "include/PinHandling.h"
#include <memory>
#include "include/TemperatureSensor.h"
//#include "../../esp32-ds18b20/include/ds18b20.h"
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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
     temp.FindDevices();
     temp.InitializeDevices();
     while(true){
     auto temperature = temp.PerformTemperatureReadOut();
     for (auto const& t : temperature){
         std::cout << "Temp is: " << t << " C" <<std::endl;
     }
         vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    }
//    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
//    uint32_t state=1;
//    void* arg=&state;
//    gpio_isr_handler_add(GPIO_NUM_23, PinHandling::ChangeStateOnImpuls, arg);
}

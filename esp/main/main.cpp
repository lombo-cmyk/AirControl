#include <stdio.h>
#include "driver/gpio.h"
#include <iostream>
#include "include/PinHandling.h"
//#include "../../esp32-ds18b20/include/ds18b20.h"
#define MAX_DEVICES          (8)
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD        (1000)   // milliseconds
#define ESP_INTR_FLAG_DEFAULT 0
extern "C"{
    void app_main();
}

void app_main(void)
{
    std::cout << "Dupa" << std::endl;
    PinHandling controllerRoutine=PinHandling();
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    uint32_t state=1;
    void* arg=&state;
    gpio_isr_handler_add(GPIO_NUM_23, PinHandling::ChangeStateOnImpuls, arg);
    controllerRoutine.Run();
}


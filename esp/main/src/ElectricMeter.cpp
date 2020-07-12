//
// Created by lukaszk on 12.07.2020.
//

#include "../include/ElectricMeter.h"

unsigned long int ElectricMeter::ElectricMeterPump = 0;
TickType_t ElectricMeter::lastWakePump = xTaskGetTickCount();

void ElectricMeter::AddPumpEnergyUsage(void* arg) {
    if ((xTaskGetTickCount() - lastWakePump) > 100){
        ElectricMeterPump++;
        lastWakePump = xTaskGetTickCount();
    }
}

void ElectricMeter::Start() {
    gpio_set_direction(electricMeterPin_1, GPIO_MODE_INPUT);
    gpio_set_direction(electricMeterPin_2, GPIO_MODE_INPUT);
    gpio_set_intr_type(electricMeterPin_1, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(electricMeterPin_2, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(electricMeterPin_1, AddPumpEnergyUsage, nullptr);
}

unsigned long int ElectricMeter::GetPumpEnergyUsage() {
    return ElectricMeterPump;
}




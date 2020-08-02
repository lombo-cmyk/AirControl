//
// Created by lukaszk on 12.07.2020.
//

#include "../include/ElectricMeter.h"

unsigned long int ElectricMeter::_ElectricMeterPump = 0;
std::uint16_t ElectricMeter::_displayState = 0;
TickType_t ElectricMeter::_lastWakeTimePump = xTaskGetTickCount();
TickType_t ElectricMeter::_lastWakeTimeForwardButton = xTaskGetTickCount();

void ElectricMeter::AddPumpEnergyUsage(void* arg) {
    if ((xTaskGetTickCount() - _lastWakeTimePump) > 100) {
        _ElectricMeterPump++;
        _lastWakeTimePump = xTaskGetTickCount();
    }
}

void ElectricMeter::DisplayNextState(void* arg) {
    if ((xTaskGetTickCount() - _lastWakeTimeForwardButton) > DEBOUNCE_TIME) {
        _displayState++;
        if (_displayState > 3) {
            _displayState = 0;
        }
        _lastWakeTimeForwardButton = xTaskGetTickCount();
    }
}

void ElectricMeter::Start() {
    gpio_set_direction(ELECTRIC_METER_PIN_1, GPIO_MODE_INPUT);
    gpio_set_direction(FORWARD_BUTTON, GPIO_MODE_INPUT);
    gpio_set_intr_type(ELECTRIC_METER_PIN_1, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(FORWARD_BUTTON, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(ELECTRIC_METER_PIN_1, AddPumpEnergyUsage, nullptr);
    gpio_isr_handler_add(FORWARD_BUTTON, DisplayNextState, nullptr);
}

std::uint64_t ElectricMeter::GetPumpEnergyUsage() {
    return _ElectricMeterPump;
}

std::uint16_t ElectricMeter::GetDisplayState() {
    return _displayState;
}

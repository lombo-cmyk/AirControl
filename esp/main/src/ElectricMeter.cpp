//
// Created by lukaszk on 12.07.2020.
//

#include "../include/ElectricMeter.h"

std::uint64_t ElectricMeter::ElectricMeterPump_ = 0;
std::uint16_t ElectricMeter::displayState_ = 0;
bool ElectricMeter::lcdBacklight_ = true;
TickType_t ElectricMeter::lastWakeTimePump_ = xTaskGetTickCount();
TickType_t ElectricMeter::lastWakeTimeForwardButton_ = xTaskGetTickCount();

void ElectricMeter::AddPumpEnergyUsage(void* arg) {
    if ((xTaskGetTickCount() - lastWakeTimePump_) > DEBOUNCE_TIME) {
        ElectricMeterPump_++;
        lastWakeTimePump_ = xTaskGetTickCount();
    }
}

void ElectricMeter::DisplayNextState(void* arg) {
    if ((xTaskGetTickCount() - lastWakeTimeForwardButton_) > DEBOUNCE_TIME) {
        displayState_++;
        if (displayState_ > 3) {
            displayState_ = 0;
        }
        lastWakeTimeForwardButton_ = xTaskGetTickCount();
    }
}

void ElectricMeter::SetLcdBacklight(void* arg) {
    lcdBacklight_ = true;
}

void ElectricMeter::Start() {
    gpio_set_direction(ELECTRIC_METER_PIN_1, GPIO_MODE_INPUT);
    gpio_set_direction(FORWARD_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(LCD_BACKLIGHT_PIN, GPIO_MODE_INPUT);
    gpio_set_intr_type(ELECTRIC_METER_PIN_1, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(FORWARD_BUTTON, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(LCD_BACKLIGHT_PIN, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(ELECTRIC_METER_PIN_1, AddPumpEnergyUsage, nullptr);
    gpio_isr_handler_add(FORWARD_BUTTON, DisplayNextState, nullptr);
    gpio_isr_handler_add(LCD_BACKLIGHT_PIN, SetLcdBacklight, nullptr);
}

std::uint64_t ElectricMeter::GetPumpEnergyUsage() {
    return ElectricMeterPump_;
}

std::uint16_t ElectricMeter::GetDisplayState() {
    return displayState_;
}

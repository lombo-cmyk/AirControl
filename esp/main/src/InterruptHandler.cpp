//
// Created by lukaszk on 12.07.2020.
//

#include "InterruptHandler.hpp"

std::uint64_t InterruptHandler::ElectricMeterPump_ = 0;
std::uint16_t InterruptHandler::displayState_ = 0;
bool InterruptHandler::lcdBacklight_ = true;
bool InterruptHandler::override_ = false;
bool InterruptHandler::useInsideAirManual_ = true;
TickType_t InterruptHandler::lastWakeTimePump_ = 0;
TickType_t InterruptHandler::lastWakeTimeForwardButton_ = 0;
TickType_t InterruptHandler::lastWakeTimeOverrideButton_ = 0;
TickType_t InterruptHandler::lastWakeTimeInsideAirManualButton_ = 0;
std::vector<class Observer*> InterruptHandler::observers_{};

void InterruptHandler::AddPumpEnergyUsage(void* arg) {
    (void) arg;
    if ((xTaskGetTickCount() - lastWakeTimePump_) > DEBOUNCE_TIME) {
        ElectricMeterPump_++;
        lastWakeTimePump_ = xTaskGetTickCount();
        NotifyObservers();
    }
}

void InterruptHandler::DisplayNextState(void* arg) {
    (void) arg;
    if ((xTaskGetTickCount() - lastWakeTimeForwardButton_) > DEBOUNCE_TIME) {
        displayState_++;
        if (displayState_ > MAX_DISPLAY_STATE) {
            displayState_ = TimeScreen;
        }
        lastWakeTimeForwardButton_ = xTaskGetTickCount();
        NotifyObservers();
    }
}

void InterruptHandler::OverrideAutomatic(void* arg) {
    (void) arg;
    if ((xTaskGetTickCount() - lastWakeTimeOverrideButton_) > DEBOUNCE_TIME) {
        if (displayState_ == AirSourceScreen) {
            override_ = !override_;
        }
        lastWakeTimeOverrideButton_ = xTaskGetTickCount();
        NotifyObservers();
    }
}

void InterruptHandler::SetAirManually(void* arg) {
    (void) arg;
    if ((xTaskGetTickCount() - lastWakeTimeInsideAirManualButton_) >
        DEBOUNCE_TIME) {
        if (displayState_ == AirSourceScreen && override_) {
            useInsideAirManual_ = !useInsideAirManual_;
        }
        lastWakeTimeInsideAirManualButton_ = xTaskGetTickCount();
        NotifyObservers();
    }
}

void InterruptHandler::Start() {
    lastWakeTimePump_ = xTaskGetTickCount();
    lastWakeTimeForwardButton_ = xTaskGetTickCount();
    lastWakeTimeOverrideButton_ = xTaskGetTickCount();
    lastWakeTimeInsideAirManualButton_ = xTaskGetTickCount();

    gpio_set_direction(ELECTRIC_METER_PIN_1, GPIO_MODE_INPUT);
    gpio_set_direction(FORWARD_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(OVERRIDE_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(AIR_SOURCE_BUTTON, GPIO_MODE_INPUT);
    gpio_pad_select_gpio(AIR_SOURCE_OUTPUT);
    gpio_set_direction(AIR_SOURCE_OUTPUT, GPIO_MODE_OUTPUT);

    gpio_set_intr_type(ELECTRIC_METER_PIN_1, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(FORWARD_BUTTON, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(OVERRIDE_BUTTON, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(AIR_SOURCE_BUTTON, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(ELECTRIC_METER_PIN_1, AddPumpEnergyUsage, nullptr);
    gpio_isr_handler_add(FORWARD_BUTTON, DisplayNextState, nullptr);
    gpio_isr_handler_add(OVERRIDE_BUTTON, OverrideAutomatic, nullptr);
    gpio_isr_handler_add(AIR_SOURCE_BUTTON, SetAirManually, nullptr);
}

std::uint64_t InterruptHandler::GetPumpEnergyUsage() {
    return ElectricMeterPump_;
}

std::uint16_t InterruptHandler::GetDisplayState() {
    return displayState_;
}
void InterruptHandler::NotifyObservers() {
    for (auto& el : observers_) {
        el->update();
    }
}
void InterruptHandler::AttachObserver(Observer* o) {
    observers_.push_back(o);
}

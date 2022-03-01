//
// Created by lukaszk on 09.07.2020.
//

#include "../include/TemperatureSensor.hpp"
#include "Free.cpp"
#include "Logger.hpp"
#include <iostream>
#include <string>
#include <mutex>

TemperatureSensor::TemperatureSensor() {
    vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    oneWireInterface_ = owb_rmt_initialize(&rmtDriverInfo_,
                                           TEMPERATURE_PIN,
                                           RMT_CHANNEL_1,
                                           RMT_CHANNEL_0);
    owb_use_crc(oneWireInterface_, true); // enable CRC check for ROM code
    owb_search_first(oneWireInterface_, &searchState_, &found_);
}

void TemperatureSensor::FindDevices() {
    LogInfo(TemperatureTag_, "Found devices:");
    while (found_) {
        std::string device = TemperatureSensor::CreateStringFromRom(
            searchState_.rom_code);
        LogInfo(TemperatureTag_, noDevices_, " : ", device);
        if (searchState_.rom_code == knownDevices_[0] ||
            searchState_.rom_code == knownDevices_[1]) {
            //            device_rom_codes[noDevices_] = searchState_.rom_code;
            noDevices_++;
        }

        owb_search_next(oneWireInterface_, &searchState_, &found_);
        totalDevicesNo_++;
    }
    if (noDevices_ != totalDevicesNo_) {
        LogInfo(TemperatureTag_,
                "Unwanted devices detected!!! Total NoDevices: ",
                totalDevicesNo_);
    }
    LogInfo(TemperatureTag_,
            "Found ",
            noDevices_,
            "device",
            (noDevices_ == 1 ? "" : "s"));

    if (noDevices_ != 2)
        esp_restart();
}

void TemperatureSensor::InitializeDevices() {
    for (int i = 0; i < noDevices_; i++) {
        auto ds18b20_info = new DS18B20_Info;
        devices_[i] = ds18b20_info;
        ds18b20_init(ds18b20_info,
                     oneWireInterface_,
                     knownDevices_[i]); // associate with bus and device
        ds18b20_use_crc(ds18b20_info, true); // enable CRC check on all reads
        ds18b20_set_resolution(ds18b20_info, DS18B20_USED_RESOLUTION);
    }
}

// Won't be used in final version
void TemperatureSensor::Run() {
    TickType_t lastWakeTime;
    if (noDevices_ > 0) {
        while (true) {
            lastWakeTime = xTaskGetTickCount();
            ds18b20_convert_all(oneWireInterface_);
            ds18b20_wait_for_conversion(devices_[0]);
            auto readings = ReadTemperature();
            if (IsErrorInReading()) {
                break;
            }
            DisplayTemperature(readings);
            vTaskDelayUntil(&lastWakeTime, SAMPLE_PERIOD / portTICK_PERIOD_MS);
        }
    }
    esp_restart();
}

std::string
TemperatureSensor::CreateStringFromRom(OneWireBus_ROMCode device) const {
    char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
    owb_string_from_rom_code(device, rom_code_s, sizeof(rom_code_s));
    return std::string(rom_code_s);
}

bool TemperatureSensor::IsErrorInReading() const {
    for (const auto err : errors_) {
        if (err != DS18B20_OK) {
            return true;
        }
    }
    return false;
}

std::array<float, MAX_DEVICES> TemperatureSensor::ReadTemperature() {
    std::array<float, MAX_DEVICES> readings{};
    for (int i = 0; i < noDevices_; ++i) {
        errors_[i] = ds18b20_read_temp(devices_[i], &readings[i]);
    }
    return readings;
}

// Won't be used in final version
template<std::size_t index>
void TemperatureSensor::DisplayTemperature(
    std::array<float, index> readings) const {
    for (int i = 0; i < noDevices_; ++i) {
        LogInfo(TemperatureTag_,
                "Temperature readings for ",
                TemperatureSensor::CreateStringFromRom(devices_[i]->rom_code),
                readings[i]++);
    }
}

std::array<float, MAX_DEVICES> TemperatureSensor::PerformTemperatureReadOut() {
    /*readings[0] -> outside temperature
     *readings[1] -> inside temperature*/
    std::mutex myMutex;
    TickType_t lastWakeTime;
    std::array<float, MAX_DEVICES> readings{};
    if (noDevices_ > 0) {
        lastWakeTime = xTaskGetTickCount();
        {
            const std::lock_guard<std::mutex> lock(myMutex);
            ds18b20_convert_all(oneWireInterface_);
            ds18b20_wait_for_conversion(devices_[0]);
            readings = ReadTemperature();
        }
        if (IsErrorInReading()) {
            throw std::runtime_error("Sensor error");
        }
        vTaskDelayUntil(&lastWakeTime, SAMPLE_PERIOD / portTICK_PERIOD_MS);
    }
    return readings;
}

//
// Created by lukaszk on 09.07.2020.
//

#include "../include/TemperatureSensor.h"
#include "Free.cpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <iostream>
#include <string>

TemperatureSensor::TemperatureSensor() {
    vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    _oneWireInterface = owb_rmt_initialize(&_rmtDriverInfo,
                                           TEMPERATURE_PIN,
                                           RMT_CHANNEL_1,
                                           RMT_CHANNEL_0);
    owb_use_crc(_oneWireInterface, true); // enable CRC check for ROM code
    owb_search_first(_oneWireInterface, &_searchState, &found);
}

void TemperatureSensor::FindDevices() {
    std::cout << "Found devices:" << std::endl;
    while (found) {
        std::string device = TemperatureSensor::CreateStringFromRom(
            _searchState.rom_code);
        std::cout << _noDevices << " : " << device << std::endl;
        if (_searchState.rom_code == _knownDevices[0] ||
            _searchState.rom_code == _knownDevices[1]) {
            //            device_rom_codes[_noDevices] = _searchState.rom_code;
            _noDevices++;
        }

        owb_search_next(_oneWireInterface, &_searchState, &found);
        _totalDevicesNo++;
    }
    if (_noDevices != _totalDevicesNo) {
        std::cout << "Unwanted devices detected!!! Total NoDevices: "
                  << _totalDevicesNo << std::endl;
    }

    std::cout << "Found " << _noDevices << "device"
              << (_noDevices == 1 ? "" : "s") << std::endl;
    if (_noDevices != 2)
        esp_restart();
}

void TemperatureSensor::InitializeDevices() {
    for (int i = 0; i < _noDevices; i++) {
        auto ds18b20_info = new DS18B20_Info;
        devices[i] = ds18b20_info;
        ds18b20_init(ds18b20_info,
                     _oneWireInterface,
                     _knownDevices[i]); // associate with bus and device
        ds18b20_use_crc(ds18b20_info, true); // enable CRC check on all reads
        ds18b20_set_resolution(ds18b20_info, DS18B20_USED_RESOLUTION);
    }
}

// Won't be used in final version
void TemperatureSensor::Run() {
    TickType_t lastWakeTime;
    if (_noDevices > 0) {
        while (true) {
            lastWakeTime = xTaskGetTickCount();
            ds18b20_convert_all(_oneWireInterface);
            ds18b20_wait_for_conversion(devices[0]);
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

bool TemperatureSensor::IsErrorInReading() {
    for (const auto err : errors) {
        if (err != DS18B20_OK) {
            return true;
        }
    }
    return false;
}

std::array<float, MAX_DEVICES> TemperatureSensor::ReadTemperature() {
    std::array<float, MAX_DEVICES> readings{};
    for (int i = 0; i < _noDevices; ++i) {
        errors[i] = ds18b20_read_temp(devices[i], &readings[i]);
    }
    return readings;
}

// Won't be used in final version
template<std::size_t index>
void TemperatureSensor::DisplayTemperature(
    std::array<float, index> readings) const {
    for (int i = 0; i < _noDevices; ++i) {
        std::cout << "Temperature readings for "
                  << TemperatureSensor::CreateStringFromRom(
                         devices[i]->rom_code)
                  << " : " << readings[i]++ << std::endl;
    }
}

std::array<float, MAX_DEVICES> TemperatureSensor::PerformTemperatureReadOut() {
    /*readings[0] -> outside temperature
     *readings[1] -> inside temperature*/
    TickType_t lastWakeTime;
    std::array<float, MAX_DEVICES> readings{};
    if (_noDevices > 0) {
        lastWakeTime = xTaskGetTickCount();
        ds18b20_convert_all(_oneWireInterface);
        ds18b20_wait_for_conversion(devices[0]);
        readings = ReadTemperature();
        if (IsErrorInReading()) {
            throw std::runtime_error("Sensor error");
        }
        vTaskDelayUntil(&lastWakeTime, SAMPLE_PERIOD / portTICK_PERIOD_MS);
    }
    return readings;
}

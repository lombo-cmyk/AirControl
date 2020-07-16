//
// Created by lukaszk on 09.07.2020.
//

#include "../include/TemperatureSensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <iostream>
#include <string>

TemperatureSensor::TemperatureSensor() {
    vTaskDelay(2000.0 / portTICK_PERIOD_MS);
    oneWireInterface = (owb_rmt_initialize(&rmt_driver_info, temperaturePin, RMT_CHANNEL_1, RMT_CHANNEL_0));
    owb_use_crc(oneWireInterface, true);  // enable CRC check for ROM code
    owb_search_first(oneWireInterface, &search_state, &found);
}

bool operator==(const OneWireBus_ROMCode &lhs, const OneWireBus_ROMCode &rhs) {
    for(int i=0; i < 8; i++){
        if(lhs.bytes[i] != rhs.bytes[i]) {
            return false;
        }
    }
    return true;
}

void TemperatureSensor::FindDevices() {
    std::cout << "Found devices:" << std::endl;
    while (found)
    {
        std::string device = TemperatureSensor::CreateStringFromRom(search_state.rom_code);
        std::cout << NoDevices << " : " << device << std::endl;
        if(search_state.rom_code == knownDevices[0] || search_state.rom_code == knownDevices[1]){
//            device_rom_codes[NoDevices] = search_state.rom_code;
            NoDevices++;
        }
        owb_search_next(oneWireInterface, &search_state, &found);
        totalDevicesNo++;
    }
    if(NoDevices != totalDevicesNo){
        std::cout << "Unwanted devices detected!!! Total NoDevices: " << totalDevicesNo << std::endl;
    }
    std::cout << "Found " << NoDevices << "device" << (NoDevices == 1 ? "" : "s") << std::endl;
    if (NoDevices != 2)
        esp_restart();
}

void TemperatureSensor::InitializeDevices() {
    for (int i=0; i < NoDevices; i++)
    {
        DS18B20_Info * ds18b20_info = ds18b20_malloc();  // heap allocation
        devices[i] = ds18b20_info;
        ds18b20_init(ds18b20_info, oneWireInterface, knownDevices[i]); // associate with bus and device
        ds18b20_use_crc(ds18b20_info, true);           // enable CRC check on all reads
        ds18b20_set_resolution(ds18b20_info, DS18B20_RESOLUTION);
    }
}

void TemperatureSensor::Run() {
    TickType_t lastWakeTime;
    if (NoDevices > 0){
        while(true){
            lastWakeTime = xTaskGetTickCount();
            ds18b20_convert_all(oneWireInterface);
            ds18b20_wait_for_conversion(devices[0]);
            auto readings = ReadTemperature();
            if (IsErrorInReading()){
                break;
            }
            DisplayTemperature(readings);
            vTaskDelayUntil(&lastWakeTime, SAMPLE_PERIOD / portTICK_PERIOD_MS);
        }
    }
    esp_restart();
}

std::string TemperatureSensor::CreateStringFromRom(OneWireBus_ROMCode device) const{
    char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
    owb_string_from_rom_code(device, rom_code_s, sizeof(rom_code_s));
    std::string ret(rom_code_s);
    return ret;
}

bool TemperatureSensor::IsErrorInReading() {
    for(const DS18B20_ERROR err: errors){
        if (err != DS18B20_OK){
            return true;
        }
    }
    return false;
}

std::array<float, MAX_DEVICES> TemperatureSensor::ReadTemperature() {
    std::array<float, MAX_DEVICES> readings{};
    for (int i = 0; i < NoDevices; ++i) {
        errors[i] = ds18b20_read_temp(devices[i], &readings[i]);
    }
    return readings;
}

template <unsigned int T>
void TemperatureSensor::DisplayTemperature(std::array<float, T> readings) const{
    for (int i = 0; i < NoDevices; ++i){
        std::cout << "Temperature readings for " << TemperatureSensor::CreateStringFromRom(
                devices[i]->rom_code) << " : " << readings[i]++ << std::endl;
    }
}

std::array<float, MAX_DEVICES> TemperatureSensor::PerformTemperatureReadOut() {
    TickType_t lastWakeTime;
    std::array<float, MAX_DEVICES> readings{};
    if (NoDevices > 0){
            lastWakeTime = xTaskGetTickCount();
            ds18b20_convert_all(oneWireInterface);
            ds18b20_wait_for_conversion(devices[0]);
            readings = ReadTemperature();
            if (IsErrorInReading()){
                throw std::runtime_error("Sensor error");
            }
            vTaskDelayUntil(&lastWakeTime, SAMPLE_PERIOD / portTICK_PERIOD_MS);
    }
    return readings;
}














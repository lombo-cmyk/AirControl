//
// Created by lukaszk on 09.07.2020.
//

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "Logger.hpp"
#include "TemperatureSensor.hpp"

std::vector<class Observer*> TemperatureSensor::observers_{};

void TemperatureSensor::Init() {
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    oneWireInterface_ = owb_rmt_initialize(&rmtDriverInfo_,
                                           TEMPERATURE_PIN,
                                           RMT_CHANNEL_1,
                                           RMT_CHANNEL_0);
    owb_use_crc(oneWireInterface_, true);
    FindDevices();
}

void TemperatureSensor::FindDevices() {
    LogInfo(temperatureTag_, "Searching for devices...");
    bool found;
    int knownDeviceCount = 0;
    int totalDevicesCount_ = 0;
    OneWireBus_SearchState searchState = {};

    owb_search_first(oneWireInterface_, &searchState, &found);
    while (found) {
        std::string device = CreateStringFromRom(searchState.rom_code);
        LogInfo(temperatureTag_, knownDeviceCount, " : ", device);

        if (sensorMap_.count(searchState.rom_code)) {
            InitializeDevice(searchState.rom_code);
            knownDeviceCount++;
        }

        owb_search_next(oneWireInterface_, &searchState, &found);
        totalDevicesCount_++;
    }
    if (knownDeviceCount != totalDevicesCount_) {
        LogInfo(temperatureTag_,
                "Unknown devices detected! Total dev count: ",
                totalDevicesCount_);
    }
    LogInfo(temperatureTag_,
            "Found ",
            knownDeviceCount,
            " known device",
            (knownDeviceCount == 1 ? "" : "s"));

    if (knownDeviceCount != 2)
        esp_restart();
}

void TemperatureSensor::InitializeDevice(const OneWireBus_ROMCode& rom) {
    auto ds18b20_info = std::make_unique<DS18B20_Info>();
    ds18b20_init(ds18b20_info.get(), oneWireInterface_, rom);
    ds18b20_use_crc(ds18b20_info.get(), true);
    ds18b20_set_resolution(ds18b20_info.get(), DS18B20_USED_RESOLUTION);

    devices_[ds18b20_info->rom_code] = std::move(ds18b20_info);
}

void TemperatureSensor::Run() {
    // TODO: adjust stack
    xTaskCreate(RunInfinity,
                temperatureTag_.begin(),
                4096,
                nullptr,
                tskIDLE_PRIORITY,
                &taskHandle_);
    configASSERT(taskHandle_);
}

void TemperatureSensor::RunInfinity(void* pvParameters) {
    (void) pvParameters;
    TickType_t lastWakeTime;
    auto& d = TemperatureSensor::getInstance();
    RomHashMap<float> readings{};

    for (;;) {
        lastWakeTime = xTaskGetTickCount();
        d.PerformTemperatureReadOut(&readings);
        d.PublishTemperature(readings);
        vTaskDelayUntil(&lastWakeTime, SAMPLE_PERIOD / portTICK_PERIOD_MS);
    }
}

std::string TemperatureSensor::CreateStringFromRom(OneWireBus_ROMCode device) {
    char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
    owb_string_from_rom_code(device, rom_code_s, sizeof(rom_code_s));
    return std::string(rom_code_s);
}

DS18B20_ERROR
TemperatureSensor::ReadTemperature(RomHashMap<float>* readings) {
    DS18B20_ERROR error = DS18B20_OK;
    for (auto const& [rom, device] : devices_) {
        float reading;
        error = static_cast<DS18B20_ERROR>(
            error | ds18b20_read_temp(device.get(), &reading));
        readings->insert_or_assign(rom, reading);
    }
    return error;
}

void TemperatureSensor::PublishTemperature(const RomHashMap<float>& readings) {
    for (auto const& [rom, string_name] : sensorMap_) {
        LogInfo(temperatureTag_,
                "Temperature readings for ",
                string_name,
                ": ",
                readings.find(rom)->second);
        readingToPublish_[string_name] = readings.find(rom)->second;
    }
    for (auto& o : observers_) {
        o->update();
    }
}

void TemperatureSensor::PerformTemperatureReadOut(
    RomHashMap<float>* readings) {
    std::mutex myMutex;
    const std::lock_guard<std::mutex> lock(myMutex);
    ds18b20_convert_all(oneWireInterface_);
    ds18b20_wait_for_conversion(devices_.find(outsideSensor_)->second.get());
    auto error = ReadTemperature(readings);
    if (error != DS18B20_OK) {
        throw std::runtime_error("Sensor error");
    }
}
void TemperatureSensor::AttachObserver(Observer* o) {
    observers_.push_back(o);
}

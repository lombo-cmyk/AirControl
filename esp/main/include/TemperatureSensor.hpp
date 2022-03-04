//
// Created by lukaszk on 09.07.2020.
//

#ifndef AIRCONTROLLER_TEMPERATURESENSOR_HPP
#define AIRCONTROLLER_TEMPERATURESENSOR_HPP

#include "Definitions.hpp"
#include "ds18b20.h"
#include "owb.h"
#include "owb_rmt.h"
#include "Singleton.h"
#include <array>
#include <cstdint>
#include <memory>
#include <map>

typedef uint8_t crc_key;

class TemperatureSensor final : public Singleton<TemperatureSensor> {
public:
    friend Singleton<TemperatureSensor>;
    void Init();
    void Run();

private:
    static constexpr std::string_view temperatureTag_ = "Temp";
    static constexpr std::string_view outsideKey_ = "outside";
    static constexpr std::string_view insideKey_ = "inside";
    std::map<crc_key, std::shared_ptr<DS18B20_Info>> devices_ = {};
    owb_rmt_driver_info rmtDriverInfo_{};

    OneWireBus* oneWireInterface_;
    TaskHandle_t taskHandle_ = nullptr;
    void InitializeDevice(const OneWireBus_ROMCode& rom);

    void FindDevices();
    void PerformTemperatureReadOut(std::map<crc_key, float>* readings) const;
    DS18B20_ERROR ReadTemperature(std::map<crc_key, float>* readings) const;

    void PublishTemperature(const std::map<crc_key, float>& readings) const;
    static std::string CreateStringFromRom(OneWireBus_ROMCode device);

    static void RunInfinity(void* pvParameters);
    OneWireBus_ROMCode outsideSensor_ = {{.family = {0x28},
                                          .serial_number =
                                              {
                                                  0x5d,
                                                  0xc2,
                                                  0x05,
                                                  0x0c,
                                                  0x00,
                                                  0x00,
                                              },
                                          .crc = {0xe1}}};

    OneWireBus_ROMCode insideSensor_ = {{.family = {0x28},
                                         .serial_number =
                                             {
                                                 0xff,
                                                 0xbf,
                                                 0xed,
                                                 0x76,
                                                 0x18,
                                                 0x01,
                                             },
                                         .crc = {0x46}}};
    std::map<crc_key, std::string_view> sensorMap_ = {
        {outsideSensor_.fields.crc[0], outsideKey_},
        {insideSensor_.fields.crc[0], insideKey_}};
};

#endif // AIRCONTROLLER_TEMPERATURESENSOR_HPP

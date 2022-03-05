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
#include "RomHashMap.h"
#include <array>
#include <cstdint>
#include <memory>

class TemperatureSensor final : public Singleton<TemperatureSensor> {
public:
    friend Singleton<TemperatureSensor>;
    void Init();
    void Run();

private:
    static constexpr std::string_view temperatureTag_ = "Temp";
    static constexpr std::string_view outsideKey_ = "outside";
    static constexpr std::string_view insideKey_ = "inside";
    RomHashMap<std::unique_ptr<DS18B20_Info>> devices_;
    owb_rmt_driver_info rmtDriverInfo_{};

    OneWireBus* oneWireInterface_;
    TaskHandle_t taskHandle_ = nullptr;
    void InitializeDevice(const OneWireBus_ROMCode& rom);

    void FindDevices();
    void PerformTemperatureReadOut(RomHashMap<float>* readings) const;
    DS18B20_ERROR ReadTemperature(RomHashMap<float>* readings) const;

    void PublishTemperature(const RomHashMap<float>& readings) const;
    static std::string CreateStringFromRom(OneWireBus_ROMCode device);

    static void RunInfinity(void* pvParameters);
    static constexpr OneWireBus_ROMCode outsideSensor_ = {{.family = {0x28},
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

    static constexpr OneWireBus_ROMCode insideSensor_ = {{.family = {0x28},
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
    RomHashMap<std::string_view> sensorMap_{{outsideSensor_, outsideKey_},
                                            {insideSensor_, insideKey_}};
};

#endif // AIRCONTROLLER_TEMPERATURESENSOR_HPP

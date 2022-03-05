//
// Created by lukaszk on 09.07.2020.
//

#ifndef AIRCONTROLLER_TEMPERATURESENSOR_HPP
#define AIRCONTROLLER_TEMPERATURESENSOR_HPP

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "ds18b20.h"
#include "owb.h"
#include "owb_rmt.h"

#include "Definitions.hpp"
#include "ObserverBase.hpp"
#include "RomHashMap.hpp"
#include "Singleton.h"

class TemperatureSensor final : public Singleton<TemperatureSensor> {
public:
    friend Singleton<TemperatureSensor>;
    void Init();
    void Run();
    auto GetTemperature() -> const StringViewHashMap& {
        return readingToPublish_;
    }
    static void AttachObserver(Observer* o);

private:
    static constexpr std::string_view temperatureTag_ = "Temp";
    static constexpr std::string_view outsideKey_ = "outside";
    static constexpr std::string_view insideKey_ = "inside";
    RomHashMap<std::unique_ptr<DS18B20_Info>> devices_;
    StringViewHashMap readingToPublish_;
    owb_rmt_driver_info rmtDriverInfo_{};
    OneWireBus* oneWireInterface_;
    TaskHandle_t taskHandle_ = nullptr;
    static std::vector<class Observer*> observers_;

    void InitializeDevice(const OneWireBus_ROMCode& rom);
    void FindDevices();
    void PerformTemperatureReadOut(RomHashMap<float>* readings);
    DS18B20_ERROR ReadTemperature(RomHashMap<float>* readings);
    void PublishTemperature(const RomHashMap<float>& readings);

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

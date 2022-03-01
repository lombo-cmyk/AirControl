//
// Created by lukaszk on 09.07.2020.
//

#ifndef AIRCONTROLLER_TEMPERATURESENSOR_HPP
#define AIRCONTROLLER_TEMPERATURESENSOR_HPP

#include "Definitions.hpp"
#include "ds18b20.h"
#include "owb.h"
#include "owb_rmt.h"
#include <array>
#include <cstdint>
#include <memory>

class TemperatureSensor {
public:
    TemperatureSensor();
    void FindDevices();
    void InitializeDevices();
    void Run();
    std::array<float, MAX_DEVICES> PerformTemperatureReadOut();

private:
    std::string CreateStringFromRom(OneWireBus_ROMCode device) const;
    bool IsErrorInReading() const;
    std::array<float, MAX_DEVICES> ReadTemperature();
    template<unsigned int T>
    void DisplayTemperature(std::array<float, T> reading) const;

    static constexpr std::string_view TemperatureTag_ = "LCD";
    int noDevices_ = 0;
    int totalDevicesNo_ = 0;
    bool found_ = false;
    owb_rmt_driver_info rmtDriverInfo_{};
    OneWireBus* oneWireInterface_;
    OneWireBus_SearchState searchState_ = {};
    DS18B20_ERROR errors_[MAX_DEVICES] = {DS18B20_OK};
    DS18B20_Info* devices_[MAX_DEVICES] = {};
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
    const std::array<OneWireBus_ROMCode, 2> knownDevices_{outsideSensor_,
                                                          insideSensor_};
    // outsideSensor_ -> 0
    // insideSensor_ -> 1
};

#endif // AIRCONTROLLER_TEMPERATURESENSOR_HPP

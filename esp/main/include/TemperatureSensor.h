//
// Created by lukaszk on 09.07.2020.
//

#ifndef AIRCONTROLLER_TEMPERATURESENSOR_H
#define AIRCONTROLLER_TEMPERATURESENSOR_H

#include "Definitions.h"
#include "ds18b20.h"
#include "owb.h"
#include "owb_rmt.h"
#include <array>
#include <cstdint>

class TemperatureSensor {
public:
    TemperatureSensor();
    void FindDevices();
    void InitializeDevices();
    void Run();
    std::array<float, MAX_DEVICES> PerformTemperatureReadOut();

private:
    std::string CreateStringFromRom(OneWireBus_ROMCode device) const;
    bool IsErrorInReading();
    std::array<float, MAX_DEVICES> ReadTemperature();
    template<unsigned int T>
    void DisplayTemperature(std::array<float, T> reading) const;
    //    bool operator==(const OneWireBus_ROMCode& rhs);
    int NoDevices = 0;
    int totalDevicesNo = 0;
    bool found = false;
    static const std::uint8_t oneWireBusBytes = 8;
    owb_rmt_driver_info rmtDriverInfo{};
    OneWireBus* oneWireInterface;
    OneWireBus_SearchState searchState = {0};
    DS18B20_ERROR errors[MAX_DEVICES] = {DS18B20_OK};
    DS18B20_Info* devices[MAX_DEVICES] = {0};
    OneWireBus_ROMCode outsideSensor = {
        {{0x28}, {0x5d, 0xc2, 0x05, 0x0c, 0x00, 0x00}, {0xe1}}};
    OneWireBus_ROMCode insideSensor = {
        {{0x28}, {0xff, 0xbf, 0xed, 0x76, 0x18, 0x01}, {0x46}},
    };
    const std::array<OneWireBus_ROMCode, 2> knownDevices{outsideSensor,
                                                         insideSensor};
    // outsideSensor -> 0
    // insideSensor -> 1
};

#endif // AIRCONTROLLER_TEMPERATURESENSOR_H

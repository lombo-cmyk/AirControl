//
// Created by lukaszk on 09.07.2020.
//

#ifndef APP_TEMPLATE_TEMPERATURESENSOR_H
#define APP_TEMPLATE_TEMPERATURESENSOR_H

#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"
#include <memory>
#include <new>
#include <string>
#include <array>
#define MAX_DEVICES          2
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD        (1000)   // milliseconds
#define DEVICE_ERROR
class TemperatureSensor {
public:
    TemperatureSensor();
    void FindDevices();
    void InitializeDevices();
    void Run();
    std::array<float, MAX_DEVICES> PerformTemperatureReadOut();
    std::string CreateStringFromRom(OneWireBus_ROMCode device) const;
    friend bool operator==(const OneWireBus_ROMCode& lhs, const OneWireBus_ROMCode& rhs);


private:
    owb_rmt_driver_info rmt_driver_info{};
    OneWireBus *oneWireInterface;
    int NoDevices = 0;
    int totalDevicesNo = 0;
    OneWireBus_SearchState search_state = {0};
    DS18B20_ERROR errors[MAX_DEVICES] = { DS18B20_OK };
    bool found = false;
    DS18B20_Info * devices[MAX_DEVICES] = {0};
    bool IsErrorInReading();
    std::array<float, MAX_DEVICES> ReadTemperature();
    template <unsigned int T> void DisplayTemperature(std::array<float, T> reading) const;
    OneWireBus_ROMCode outsideSensor = {{{0x28},
                                         {0x5d,
                                          0xc2,
                                          0x05,
                                          0x0c,
                                          0x00,
                                          0x00},
                                          {0xe1}}
    };
    OneWireBus_ROMCode insideSensor = { {{ 0x28 },
                                                          { 0xff,
                                                            0xbf,
                                                            0xed,
                                                            0x76,
                                                            0x18,
                                                            0x01 },
                                                        { 0x46 }},
    };
    const std::array<OneWireBus_ROMCode, 2> knownDevices{outsideSensor, insideSensor};
    //outsideSensor -> 0
    //insideSensor -> 1
};

#endif //APP_TEMPLATE_TEMPERATURESENSOR_H

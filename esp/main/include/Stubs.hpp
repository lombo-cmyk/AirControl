//
// Created by Lukasz on 01.03.2022.
//

#ifndef AIRCONTROLLER_STUBS_H
#define AIRCONTROLLER_STUBS_H

#include "Definitions.hpp"

class LCD {
public:
    static void AdjustLine(std::string& line){};
    void DisplayScreen(std::array<float, MAX_DEVICES>& temp){};
};

class TemperatureSensor {
public:
    void FindDevices(){};

    void InitializeDevices(){};

    void Run(){};

    std::array<float, MAX_DEVICES> PerformTemperatureReadOut() {
        a[0] += .1;
        a[1] += .1;
        return a;
    };

private:
    std::array<float, MAX_DEVICES> a{1.1, 2.2};
};

#endif // AIRCONTROLLER_STUBS_H

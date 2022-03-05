#include <ctime>
#include <iomanip>
#include <sstream>
#include "esp_log.h"
#include "InterruptHandler.hpp"
#include "Wifi.hpp"
#include "Logger.hpp"
#include "AirControlMotor.hpp"

#ifdef CONFIG_INCLUDE_STUBS

#include "Stubs.hpp"

#else
#include "LCD.hpp"
#include "TemperatureSensor.hpp"
#endif

extern "C" {
void app_main();
}

void app_main(void) {
    LogInfo("main", "Esp starting");
    auto &temp = TemperatureSensor::getInstance();
    temp.Init();
    InterruptHandler::Start();
    LCD LCDisplay = LCD();
    Wifi::StartWifi();
    std::array<float, MAX_DEVICES> temperature = {1.1, 2.2};
    AirControlMotor Motor;
    temp.Run();
    for (;;) {
        LCDisplay.DisplayScreen(temperature);
        LogInfo("main", "kWh Pump: ", InterruptHandler::GetPumpEnergyUsage());
        LogInfo("main", "LCD State: ", InterruptHandler::GetDisplayState());
        Motor.SetMotor(temperature,
                       InterruptHandler::GetOverride(),
                       InterruptHandler::GetManualInfo());

        std::time_t t = std::time(nullptr);
        std::ostringstream stream;
        stream << std::put_time(std::localtime(&t), "%d-%m-%Y %H:%M:%S");
        LogInfo("main", "The current date/time is: ", stream.str());
        vTaskDelay(10000.0 / portTICK_PERIOD_MS);
    }
}


/* Add it to readme
  thanks for the reports, but unfortunately failed to reproduce the issue. Could you please try to add --target=riscv32-unknown-unknown-unknown to the list of clangd options? (File | Settings | Languages & Frameworks | C/C++ | Clangd, field under Clang Errors And Warnings).
@Jens Otto, any chance you can go into file with errors and do Help | Find Actions | clangd: report bug? Also please try the option above */
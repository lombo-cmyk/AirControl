#include <ctime>
#include <iomanip>
#include <sstream>

#include "esp_log.h"

#include "AirControlMotor.hpp"
#include "I2CWrapper.hpp"
#include "InterruptHandler.hpp"
#include "Logger.hpp"
#include "Wifi.hpp"

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
    auto& temp = TemperatureSensor::getInstance();
    temp.Init();
    temp.Run();

    InterruptHandler::Start();

    auto& d = I2CWrapper::getInstance();
    d.Init(LCD_SDA_PIN, LCD_SCL_PIN);

    auto lcd = LCD();
    lcd.Init(LCD_COLUMNS, LCD_ADDRESS);
    lcd.DisplayWelcomeMessage();

    Wifi::StartWifi();

    AirControlMotor Motor;
    Motor.Init();

    for (;;) {
        lcd.DisplayCurrentState();

        LogInfo("main", "kWh Pump: ", InterruptHandler::GetPumpEnergyUsage());
        LogInfo("main", "LCD State: ", InterruptHandler::GetDisplayState());

        std::time_t t = std::time(nullptr);
        std::ostringstream stream;
        stream << std::put_time(std::localtime(&t), "%d-%m-%Y %H:%M:%S");
        LogInfo("main", "The current date/time is: ", stream.str());
        vTaskDelay(10000.0 / portTICK_PERIOD_MS);
    }
}

/* Add it to readme
  thanks for the reports, but unfortunately failed to reproduce the issue.
Could you please try to add --target=riscv32-unknown-unknown-unknown to the
list of clangd options? (File | Settings | Languages & Frameworks | C/C++ |
Clangd, field under Clang Errors And Warnings).
@Jens Otto, any chance you can go into file with errors and do Help | Find
Actions | clangd: report bug? Also please try the option above */
//
// Created by Lukasz on 27.02.2022.
//

#ifndef AIRCONTROLLER_LOGGER_H
#define AIRCONTROLLER_LOGGER_H
#include <string>
#include <iomanip>
#include "sntp.h"
#include "esp_log.h"
#include <cstdarg>

template<typename... Targs>
std::string PrepareLogString(Targs... Fargs){
    std::ostringstream ss1;
    (ss1 << ... << Fargs);
    return ss1.str();
}

template<typename... Targs>
void LogInfo(std::string_view Tag, Targs... Fargs){
    std::string s;
    s = PrepareLogString(Fargs...);
    ESP_LOGI(Tag.begin(), "%s", s.c_str());
}

#endif //AIRCONTROLLER_LOGGER_H

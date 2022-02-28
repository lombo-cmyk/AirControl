//
// Created by Lukasz on 27.02.2022.
//

#ifndef AIRCONTROLLER_LOGGER_H
#define AIRCONTROLLER_LOGGER_H
#include <string>
#include "sntp.h"

void LogInfo(std::string_view tag, const std::string& logInfo);

template<typename T>
void LogInfo(std::string_view tag, const std::string &logInfo, T reading){
    ESP_LOGI(tag.begin(), "%s %d", logInfo.c_str(), reading);
}

template<>
void LogInfo<std::ostringstream*>(std::string_view tag, const std::string &logInfo, std::ostringstream *reading);

template<>
void LogInfo<std::string*>(std::string_view tag, const std::string &logInfo, std::string *reading);

#endif //AIRCONTROLLER_LOGGER_H

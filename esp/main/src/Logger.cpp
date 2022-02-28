//
// Created by Lukasz on 27.02.2022.
//

#include "Logger.hpp"
#include "esp_log.h"
#include <iomanip>
#include <string_view>


void LogInfo(std::string_view tag, const std::string& logInfo){
    ESP_LOGI(tag.begin(), "%s", logInfo.c_str());
}

template<>
void LogInfo<std::ostringstream*>(std::string_view tag, const std::string &logInfo, std::ostringstream *reading){
    ESP_LOGI(tag.begin(), "%s %s", logInfo.c_str(), reading->str().c_str());
}

template<>
void LogInfo<std::string*>(std::string_view tag, const std::string &logInfo, std::string *reading){
    ESP_LOGI(tag.begin(), "%s %s", logInfo.c_str(), reading->c_str());
}

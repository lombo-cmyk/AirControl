//
// Created by Lukasz on 26.02.2022.
//
#include <bitset>
#include <string>
#include "esp_sntp.h"

#include "esp_log.h"
#include "Definitions.hpp"

#include "Sntp.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

EventGroupHandle_t Sntp::sntpEventGroup_{};

void Sntp::SyncTime() {
    sntpEventGroup_ = xEventGroupCreate();
    InitializeSntp();

    std::bitset<sizeof(EventBits_t) * 8> bits = xEventGroupWaitBits(sntpEventGroup_,
                                                                    timeSynchronizedBit_,
                                                                    pdFALSE,
                                                                    pdFALSE,
                                                                    SECOND * 10);
    if (!bits[timeSynchronizedIndex_]){
        ESP_LOGI(SntpTag_, "Failed to synchronize time. Sync status: %d", sntp_get_sync_status());
    }

}

void Sntp::InitializeSntp() {
    ESP_LOGI(SntpTag_, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(TimeSetCallback);
    sntp_init();
    if (sntp_getservername(0)){
        ESP_LOGI(SntpTag_, "Server: %s", sntp_getservername(0));
    }
}

void Sntp::TimeSetCallback(struct timeval *tv) {
    auto tm = *std::localtime(&tv->tv_sec);
    std::ostringstream stream;
    stream << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    ESP_LOGI(SntpTag_, "Time sync event. Time: %s", stream.str().c_str());

    xEventGroupSetBits(sntpEventGroup_, timeSynchronizedBit_);
}



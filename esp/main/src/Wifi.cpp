//
// Created by Lukasz on 26.02.2022.
//
#include <cstring>
#include <bitset>

#include "esp_event.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "Logger.hpp"
#include "Sntp.hpp"
#include "Wifi.hpp"

uint8_t Wifi::numberOfConnRetries_ = 0;
EventGroupHandle_t Wifi::wifiEventGroup_{};

/*  ******** Callbacks ******** */
void Wifi::WifiEventCallback(void* arg,
                             esp_event_base_t eventBase,
                             int32_t eventId,
                             void* eventData) {
    (void) arg;
    (void) eventBase;
    (void) eventData;

    switch (eventId) {
    case WIFI_EVENT_STA_START: {
        esp_wifi_connect();
        break;
    }
    case WIFI_EVENT_STA_DISCONNECTED: {
        if (numberOfConnRetries_ < pMaxRetry_) {
            esp_wifi_connect();
            numberOfConnRetries_++;
            LogInfo(WifiTag_,
                    "Trying to connect to AP...",
                    (int) numberOfConnRetries_,
                    "/",
                    pMaxRetry_);
        } else {
            xEventGroupSetBits(wifiEventGroup_, connectedFailBit_);
            LogInfo(WifiTag_, "AP connection failed");
        }
        break;
    }
    default: {
        LogInfo(WifiTag_, "Uncovered event: ", eventId);
    }
    }
}

void Wifi::IpEventCallback(void* arg,
                           esp_event_base_t eventBase,
                           int32_t eventId,
                           void* eventData) {
    (void) arg;
    (void) eventBase;
    (void) eventId;
    auto* event = (ip_event_got_ip_t*) eventData;
    char buffer[16];
    sprintf(buffer, IPSTR, IP2STR(&event->ip_info.ip));
    LogInfo(WifiTag_, "Connected. IP: ", buffer);
    numberOfConnRetries_ = 0;
    xEventGroupSetBits(wifiEventGroup_, Wifi::connectedBit_);
}
/* ******** End callbacks ******** */

/* ******** Public ******** */
void Wifi::StartWifi() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    LogInfo(WifiTag_, "Starting Station mode.");
    Wifi::WifiInitStation();
    Sntp::SyncTime();
}
/* ******** End Public ******** */

/* ******** Private ******** */
void Wifi::WifiInitStation() {
    wifiEventGroup_ = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    EspWifiInit();

    wifi_config_t wifiConfig{};
    ConfigureConnection(&wifiConfig);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    ConnectToAP();

    vEventGroupDelete(wifiEventGroup_);
}

void Wifi::EspWifiInit() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT()
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void Wifi::ConfigureConnection(wifi_config_t* wifiConfig) {
    wifi_sta_config_t wifiStationCfg = {};
    memcpy(wifiStationCfg.ssid, ssid_.begin(), ssid_.length() + 1);
    memcpy(wifiStationCfg.password, password.begin(), password.length() + 1);
    wifiStationCfg.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    wifiConfig->sta = wifiStationCfg;
}

void Wifi::ConnectToAP() {
    esp_event_handler_instance_t wifiEventHandler;
    esp_event_handler_instance_t ipEventHandler;
    RegisterHandlers(&wifiEventHandler, &ipEventHandler);

    ESP_ERROR_CHECK(esp_wifi_start());

    LogInfo(WifiTag_, "Init cfg finished. Connecting...");
    WaitForConnection();

    UnregisterHandlers(&wifiEventHandler, &ipEventHandler);
}

void Wifi::RegisterHandlers(esp_event_handler_instance_t* wifiEvent,
                            esp_event_handler_instance_t* ipEvent) {
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(WIFI_EVENT,
                                            ESP_EVENT_ANY_ID,
                                            &Wifi::WifiEventCallback,
                                            nullptr,
                                            wifiEvent));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &Wifi::IpEventCallback,
                                                        nullptr,
                                                        ipEvent));
}

void Wifi::WaitForConnection() {
    std::bitset<sizeof(EventBits_t)* 8> bits = xEventGroupWaitBits(
        wifiEventGroup_,
        connectedBit_ | connectedFailBit_,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);

    if (bits[connectedBitIndex_]) {
        LogInfo(WifiTag_,
                "connected to AP SSID: ",
                ssid_,
                " password: *** :)");
    } else if (bits[connectedFailBitIndex_]) {
        LogInfo(WifiTag_,
                "Failed to connect to SSID: ",
                ssid_,
                " password: *** :)");
    }
}

void Wifi::UnregisterHandlers(esp_event_handler_instance_t* wifiEvent,
                              esp_event_handler_instance_t* ipEvent) {
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT,
                                                          IP_EVENT_STA_GOT_IP,
                                                          wifiEvent));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT,
                                                          ESP_EVENT_ANY_ID,
                                                          ipEvent));
}

/* ******** End Private ******** */

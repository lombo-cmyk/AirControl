//
// Created by Lukasz on 26.02.2022.
//
#include "freertos/event_groups.h"
#include "esp_event.h"

#ifndef AIRCONTROLLER_WIFI_H
#define AIRCONTROLLER_WIFI_H


class Wifi {
public:
    static void StartWifi();
private:
    static constexpr char *WifiTag_ = (char*)"Wifi connection";

    static constexpr char *pSsid_ = (char*)CONFIG_ESP_WIFI_SSID;
    static constexpr char *pPassword_ = (char*)CONFIG_ESP_WIFI_PASSWORD;
    static constexpr int pMaxRetry_ = CONFIG_ESP_MAXIMUM_RETRY;

    static constexpr u_int8_t connectedBit_ = BIT0;
    static constexpr u_int8_t connectedFailBit_ = BIT1;
    static constexpr u_int8_t connectedBitIndex_ = connectedBit_ - 1;
    static constexpr u_int8_t connectedFailBitIndex_ = connectedFailBit_ - 1;

    static uint8_t numberOfConnRetries_;
    static EventGroupHandle_t wifiEventGroup_;

    static void WifiEventCallback(void* arg, esp_event_base_t eventBase,
                                  int32_t eventId, void* eventData);
    static void IpEventCallback(void* arg, esp_event_base_t eventBase,
                                int32_t eventId, void* eventData);

    static void WifiInitStation();
    static void EspWifiInit();
    static void ConfigureConnection(wifi_config_t *wifiConfig);
    static void ConnectToAP();
    static void RegisterHandlers(esp_event_handler_instance_t *wifiEvent, esp_event_handler_instance_t *ipEvent);
    static void WaitForConnection();
    static void UnregisterHandlers(esp_event_handler_instance_t *wifiEvent, esp_event_handler_instance_t *ipEvent);
};


#endif //AIRCONTROLLER_WIFI_H

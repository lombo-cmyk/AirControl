//
// Created by Lukasz on 26.02.2022.
//

#ifndef AIRCONTROLLER_SNTP_H
#define AIRCONTROLLER_SNTP_H

#include "freertos/event_groups.h"

class Sntp {
public:
    static void SyncTime();
private:
    static constexpr char *SntpTag_ = (char*)"Sntp";
    static EventGroupHandle_t sntpEventGroup_;

    static constexpr u_int8_t timeSynchronizedBit_ = BIT0;
    static constexpr u_int8_t timeSynchronizedIndex_ = timeSynchronizedBit_ - 1;

    static void InitializeSntp();
    static void TimeSetCallback(struct timeval *tv);

};


#endif //AIRCONTROLLER_SNTP_H

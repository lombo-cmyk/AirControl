//
// Created by lukaszk on 30.07.2020.
//
#include "ds18b20.h"
#include <cstddef>
const std::uint8_t oneWireBusBytes = 8;
bool operator==(const OneWireBus_ROMCode& lhs, const OneWireBus_ROMCode& rhs) {
    for (int i = 0; i < oneWireBusBytes; i++) {
        if (lhs.bytes[i] != rhs.bytes[i]) {
            return false;
        }
    }
    return true;
}

// void* operator new(std::size_t count) {
//    void* p = ds18b20_malloc();
//    return p;
//}
// void operator delete(void* p) {
//    free(p);
//}
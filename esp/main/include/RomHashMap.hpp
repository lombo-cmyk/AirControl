//
// Created by Lukasz on 05.03.2022.
//

#ifndef AIRCONTROLLER_ROMHASHMAP_HPP
#define AIRCONTROLLER_ROMHASHMAP_HPP
#include <unordered_map>

struct RomHash {
    std::size_t operator()(const OneWireBus_ROMCode& k) const {
        return std::hash<uint8_t>()(k.fields.crc[0]);
    }
};

struct RomEqual {
    bool operator()(const OneWireBus_ROMCode& lhs,
                    const OneWireBus_ROMCode& rhs) const {
        return lhs.fields.crc[0] == rhs.fields.crc[0];
    }
};

template<typename T>
using RomHashMap = std::
    unordered_map<OneWireBus_ROMCode, T, RomHash, RomEqual>;

#endif // AIRCONTROLLER_ROMHASHMAP_HPP

//
// Created by lukaszk on 30.07.2020.
//

bool operator==(const OneWireBus_ROMCode& lhs, const OneWireBus_ROMCode& rhs) {
    for (int i = 0; i < 8; i++) {
        if (lhs.bytes[i] != rhs.bytes[i]) {
            return false;
        }
    }
    return true;
}
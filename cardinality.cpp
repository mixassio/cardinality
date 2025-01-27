#include "main.hpp"

ui32 CalculateCRC32(const std::string& data) {
    static const uint32_t polynomial = 0xEDB88320;
    ui32 crc = 0xFFFFFFFF;

    for (unsigned char byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; ++i) {
            if (crc & 1) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc >>= 1;
            }
        }
    }

    return ~crc;
}

// one random string of the predefined length
std::string GenerateRandomString(std::mt19937& gen, std::uniform_int_distribution<>& dist) {
    std::string s;
    s.reserve(STR_LEN);
    for (size_t i = 0; i < STR_LEN; ++i) {
        char c = static_cast<char>('a' + dist(gen) % ALFABET_SIZE);
        s.push_back(c);
    }
    return s;
}

// a set of randomly generated unique strings
std::set<std::string> GenerateStringSet(std::mt19937& gen) {
    std::uniform_int_distribution<> dist(0, ALFABET_SIZE - 1);
    std::set<std::string> uniqueStrings;
    while (uniqueStrings.size() < UNIQ_STR_SET_SIZE) {
        uniqueStrings.insert(GenerateRandomString(gen, dist));
    }
    return uniqueStrings;
}
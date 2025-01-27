#include <vector>
#include <array>
#include <cstdint>
#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <set>

using ui32 = uint32_t;

const size_t STR_LEN = 20;

uint32_t CalculateCRC32(const std::string& data) {
    static const uint32_t polynomial = 0xEDB88320;
    uint32_t crc = 0xFFFFFFFF;

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

// функция создает строку случайных символов
std::string GenerateRandomString(size_t length, std::mt19937& gen, std::uniform_int_distribution<>& dist) {
    std::string s;
    s.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        char c = static_cast<char>('a' + dist(gen) % 26);
        s.push_back(c);
    }
    return s;
}

// функция генерирует массив из count_unique_str уникальных CRC32 ключей
std::vector<std::uint32_t> GenerateUniqueSetForUsing(size_t count_unique_str, std::mt19937& gen) {
    std::vector<std::string> s;
    s.reserve(count_unique_str);
    // Генератор равномерного распределения - здесь равномерное распределение подходит, нам нужны уникальные строки
    std::uniform_int_distribution<> dist(0, 25); // 
    for (size_t i = 0; i < count_unique_str; ++i) {
        std::string c = GenerateRandomString(STR_LEN, gen, dist);
        s.push_back(c);
    }
    
    // Преобразование в std::set - на всякий случай чтобы удалить повторы
    std::set<std::string> s1(s.begin(), s.end());
    // преобразование обратно в vector
    std::vector<std::string> uniqSet(s1.begin(), s1.end());

    // В результате нам нужно получить сет CRC32
    std::vector<uint32_t> result(uniqSet.size());
    // Лямбда-функция для преобразования числа в строку
    auto stringToSrc32 = [](std::string str) {
        return CalculateCRC32(str);
    };

    std::transform(uniqSet.begin(), uniqSet.end(), result.begin(), stringToSrc32);

    return result;
}
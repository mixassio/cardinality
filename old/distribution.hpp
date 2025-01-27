
#include <vector>
#include <string>
#include <random>
#include <map>
#include <iostream>
#include <algorithm>
#include <iomanip>


// **** binominal *****
std::vector<std::uint32_t> createSetBinominal(size_t sizeResult, std::vector<std::uint32_t> &uniqueSet, std::mt19937& gen) {
    int sizeUniq = uniqueSet.size();

    // Параметры биномиального распределения
    // t = 100000 (sizeUniq), p = 0.1
    std::binomial_distribution<> d(sizeUniq, 0.1);
 
    std::vector<std::uint32_t> result;
    result.reserve(sizeResult);

    for (int n = 0; n != sizeResult; ++n) {
        int value = std::clamp(d(gen), 0, 10000);
        result.push_back(value);
    }
    return result;
}



// **** poisson ****
std::vector<std::uint32_t> createSetPoisson(size_t sizeResult, std::vector<std::uint32_t> &uniqueSet, std::mt19937& gen) {
    int sizeUniq = uniqueSet.size();

    // Устанавливаем среднее значение (λ = 5000)
    std::poisson_distribution<> d(sizeUniq / 2);
 
    std::vector<std::uint32_t> result;
    result.reserve(sizeResult);

    for (int n = 0; n != sizeResult; ++n) {
        int value = std::clamp(d(gen), 0, 10000);
        result.push_back(value);
    }
    return result;
}



// **** normal (Gaussian) ****
std::vector<std::uint32_t> createSetNormal(size_t sizeResult, std::vector<std::uint32_t> &uniqueSet, std::mt19937& gen) {
    // Среднее значение 5000, стандартное отклонение 1666
    std::normal_distribution d{5000.0, 1666.0};
 
    // Функция для генерации случайных чисел из нормального распределения
    // auto random_int = [&d, &gen]() {
    //     double value = std::round(d(gen));
    //     // Ограничиваем диапазон от 0 до 10000
    //     return std::clamp(static_cast<int>(value), 0, 10000);
    // };
 
    std::vector<std::uint32_t> result;

    for (int n = 0; n != sizeResult; ++n) {
        // int value = random_int();
        int value = static_cast<int>(std::round(d(gen)));
        if (value > 0 && value < 9999) {
            result.push_back(value);
        } 
    }

    return result;
}
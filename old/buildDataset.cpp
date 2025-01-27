#include <iostream>
#include "generateUniqueSet.hpp"
#include "distribution.hpp"
#include <chrono>
#include <map>


using ui16 = uint16_t;
using ui32 = uint32_t;
using ui64 = uint64_t;


// I ≈ sum(hist1[i]*hist2[i]) - (n1 * n2) / BUCKETS
ui64 EstimateJoinCardinality(const std::vector<ui32>& hashes1, const std::vector<ui32>& hashes2, int BUCKETS) {
    std::vector<ui16> histogram1;
    std::vector<ui16> histogram2;
    

    for (int k = 0; k < BUCKETS; k++) {
        histogram1.push_back(0);
        histogram2.push_back(0);
    }

    for (auto h : hashes1) {
        ui16 bucketIdx = (h >> 11) % BUCKETS;
        // ui16 bucketIdx = h % BUCKETS;
        histogram1[bucketIdx]++;
    }
    for (auto h : hashes2) {
        ui16 bucketIdx = (h >> 11) % BUCKETS;
        // ui16 bucketIdx = h % BUCKETS;
        histogram2[bucketIdx]++;
    }

    ui64 dot_product = 0;
    for (size_t i = 0; i < BUCKETS; i++) {
        dot_product += (uint64_t)histogram1[i] * (uint64_t)histogram2[i];
    }

    // Adjusted estimate
    double estimate = (double)dot_product - (double)(hashes1.size() * hashes2.size()) / (double)BUCKETS;
    if (estimate < 0) estimate = 0;

    return estimate;
}


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    const size_t COUNT_UNIQUE_STR = 10000;
    // создаем массив уникальный ключей из которого будем строить распределение
    std::vector<std::uint32_t> uniqueSet = GenerateUniqueSetForUsing(COUNT_UNIQUE_STR, gen);

    // строим гистограмму ошибки по SIZE экспериментам
    const size_t SIZE = 10000;
    std::vector<double> numbers(SIZE);
    const size_t TOTAL_SIZE = 50000;

    for (size_t i = 0; i < SIZE; ++i) {
        // создаем два сета для джоина с использованием различных распределений из созданного ранее уникального сета
        std::vector<std::uint32_t> setRight = createSetNormal(TOTAL_SIZE, uniqueSet, gen);
        std::vector<std::uint32_t> setLeft = createSetNormal(TOTAL_SIZE, uniqueSet, gen);
        // рассчитываем на большом количестве баккетов
        ui64 realCardinality = EstimateJoinCardinality(setRight, setLeft, TOTAL_SIZE * 0.9);
        ui64 estimated = EstimateJoinCardinality(setRight, setLeft, 1024);
        double estimatedCardinality = static_cast<double>(estimated);
        double errorPercent = std::abs(estimatedCardinality - realCardinality) / realCardinality * 100.0;
        numbers[i] = errorPercent;
        // вывод информативный, чтобы видеть прогресс
        if (i % 1000 == 0) {
            std::cout << "errorPercent    " << errorPercent << std::endl;
            std::cout << "realCardinality " << realCardinality << std::endl;
            std::cout << "estimated       " << estimated << std::endl;
        }
    }

    // // Создаем массив для подсчета чисел по диапазонам
    const int NUM_BUCKETS = 10; // Количество диапазонов
    std::vector<double> buckets(NUM_BUCKETS, 0);

    // // Подсчет количества чисел в каждом диапазоне
    for (double num : numbers) {
        int bucket_index = num * 1000; // Определяем индекс диапазона
        if (bucket_index >= NUM_BUCKETS) {
            bucket_index = NUM_BUCKETS - 1; // На случай чисел, равных 100
        }
        buckets[bucket_index]++;
    }

    // Вывод результатов
    std::cout << "Results for count BUCKETS = " << 1024 << "; bucketIdx = h % BUCKET" << std::endl;
    for (size_t i = 0; i < NUM_BUCKETS; ++i) {
        std::cout << "Range " << i << "-" << (i + 1)
                  << ": " << buckets[i] << " numbers" << std::endl;
    }

    return 0;
}
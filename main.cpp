#include <chrono>
#include <iostream>
#include <numeric>
#include "main.hpp"

int main() {

    // 1. Generate a set of unique strings
    std::random_device rd;
    std::mt19937 gen(rd());
    const std::set<std::string> unique_strings = GenerateStringSet(gen);

    // 2. Generate data sets of the chosen distribution
    // t = 100000, p = 0.1
    std::binomial_distribution<int> d(10000, 0.5);
    // λ = 5000
    std::poisson_distribution<int> d1(5000);
    // Среднее значение 5000, стандартное отклонение 1666
    std::normal_distribution<double> d2{5000.0, 1666.0};

    std::vector<ui32> set1, set2;
    std::vector<ui64> cardinalities;
    std::vector<double> times;
    for(int i = 0; i < NUM_ESTIMS; i++) {
        set1 = GenerateSubSetFromPickedIndexes<std::binomial_distribution<int>>(unique_strings, gen, d);
        set2 = GenerateSubSetFromPickedIndexes<std::normal_distribution<double>>(unique_strings, gen, d2);

        // 3. Estimate join cardinality & measure time for performance
        auto start = std::chrono::high_resolution_clock::now();
        cardinalities.push_back(EstimateJoinCardinality(set1, set2));
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        times.push_back(diff.count());
    }

    // Also keep track of total bytes processed for performance measurement
    ui64 totalBytes = set1.size() * sizeof(ui32) + set2.size() * sizeof(ui32);

    // cardinality estimates mean
    ui64 actualCardinality = std::accumulate(cardinalities.begin(), cardinalities.end(), 0) / NUM_ESTIMS;

    // 4. Calculate errors
    std::vector<double> errors;
    for(int i = 0; i < NUM_ESTIMS; i++)
        errors.push_back((double)std::abs((int)cardinalities[i] - (int)actualCardinality) / (int)actualCardinality * 100.0);

    // 5. Performance: bytes per second

    // Print results
    for(int i = 0; i < NUM_ESTIMS; i++) {
        std::cout << "Actual intersection cardinality: " << actualCardinality << std::endl;
        std::cout << "Estimated intersection cardinality: " << cardinalities[i] << std::endl;
        std::cout << "Error: " << errors[i] << " %" << std::endl;
        std::cout << "Performance: " << totalBytes / times[i] << " bytes/second" << std::endl;
    }

    return 0;
}
#include <array>
#include <cmath>
#include "main.hpp"

using ui16 = uint16_t;
using ui32 = uint32_t;
using ui64 = uint64_t;

// Build test data with known overlap
void BuildDataSets(
    std::vector<std::string>& set1,
    std::vector<std::string>& set2,
    size_t totalSize,
    size_t overlapCount) {   
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 25);

        // Generate 'totalSize' distinct strings for set1
        // fix each string length to 20 chars for simplicity
        // const size_t STR_LEN = 20;
        set1.reserve(totalSize);
        for (size_t i = 0; i < totalSize; i++) {
            set1.push_back(GenerateRandomString(gen, dist));
        }

        // Ensure overlapCount strings are the same in set2
        set2.reserve(totalSize);
        // First, copy overlapCount elements from set1 to ensure overlap
        for (size_t i = 0; i < overlapCount; i++) {
            set2.push_back(set1[i]);  // guaranteed overlap
        }
        // Then add distinct new strings to set2
        for (size_t i = overlapCount; i < totalSize; i++) {
            set2.push_back(GenerateRandomString(gen, dist));
        }
}

// Cardinality estimation function
// Simple histogram-based approach: sum over i: hist1[i]*hist2[i]
ui64 EstimateJoinCardinalityOld(const std::vector<ui32>& hashes1, const std::vector<ui32>& hashes2) {
    std::array<ui16, BUCKETS> histogram1 = {};
    std::array<ui16, BUCKETS> histogram2 = {};

    for (auto h : hashes1) {
        ui16 bucketIdx = (h % BUCKETS);
        histogram1[bucketIdx]++;
    }
    for (auto h : hashes2) {
        ui16 bucketIdx = (h % BUCKETS);
        histogram2[bucketIdx]++;
    }

    ui64 joinCardinality = 0;
    for (size_t i = 0; i < BUCKETS; i++) {
        joinCardinality += static_cast<ui64>(histogram1[i]) * static_cast<ui64>(histogram2[i]);
    }

    return joinCardinality;
}

// I ≈ sum(hist1[i]*hist2[i]) - (n1 * n2) / BUCKETS
ui64 EstimateJoinCardinality(const std::vector<ui32>& hashes1, const std::vector<ui32>& hashes2) {
    std::array<ui16, BUCKETS> histogram1 = {};
    std::array<ui16, BUCKETS> histogram2 = {};

    for (auto h : hashes1) {
        ui16 bucketIdx = (h % BUCKETS);
        histogram1[bucketIdx]++;
    }
    for (auto h : hashes2) {
        ui16 bucketIdx = (h % BUCKETS);
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

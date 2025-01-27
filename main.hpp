#pragma once

#include <cmath>
#include <vector>
#include <set>
#include <string>
#include <random>
#include <cstddef>
#include <cstdint>

using ui16 = uint16_t;
using ui32 = uint32_t;
using ui64 = uint64_t;

const int BUCKETS = 10; // 128, 256, 512, 1024
const size_t STR_LEN = 20;
const int ALFABET_SIZE = 26;
// making max amount of unique strings in the starting set 20^26
const size_t START_INDEX = 0;
const size_t UNIQ_STR_SET_SIZE = 50000;
const size_t SET_SIZE = 30000; // 30 or 80 k. Idea - more 50 k or less 50 k
const size_t TOTAL_SIZE = 50000;
const int NUM_ESTIMS = 100;

const double OVERLAP_PERCENT = 0.50;
const size_t OVERLAP_COUNT = static_cast<size_t>(TOTAL_SIZE * OVERLAP_PERCENT);

ui32 CalculateCRC32(const std::string& data);
std::string GenerateRandomString(std::mt19937& gen, std::uniform_int_distribution<>& dist);
std::set<std::string> GenerateStringSet(std::mt19937& gen);

template <typename Distribution>
size_t PickIndexFromDistribution(Distribution& dist, std::mt19937& gen) {
    // Handle any distribution that returns integral values
    using DistType = typename std::decay<Distribution>::type;
    
    if constexpr (std::is_integral_v<typename DistType::result_type>) {
        // For integral distributions like uniform_int_distribution
        return dist(gen) % SET_SIZE;
    } else if constexpr (std::is_floating_point_v<typename DistType::result_type>) {
        // For distributions like normal_distribution (which returns floating-point numbers)
        return static_cast<size_t>(std::llround(std::max(dist(gen), 0.0))) % UNIQ_STR_SET_SIZE;
    }
    return 0;  // Fallback in case of unsupported distribution type
}

// A set of strings picked using a predefined distribution from a set of unique strings
template <typename Distribution>
std::vector<ui32> GenerateSubSetFromPickedIndexes(const std::set<std::string>& stringSet, std::mt19937& gen, Distribution& dist) {
    // Convert set to vector for index-based access
    std::vector<std::string> stringVec(stringSet.begin(), stringSet.end());
    std::vector<ui32> subset;
    size_t pickedIndex;

    // Loop through the required number of selections
    for (size_t i = 0; i < SET_SIZE; ++i) {
        pickedIndex = PickIndexFromDistribution(dist, gen);
        // Pick and convert strings to CRC32 hashes
        subset.push_back(CalculateCRC32(stringVec[pickedIndex]));
    }
    return subset;
}

void BuildDataSets(std::vector<std::string>& set1, std::vector<std::string>& set2,
                   size_t totalSize, size_t overlapCount);
ui64 EstimateJoinCardinalityOld(const std::vector<ui32>& hashes1, const std::vector<ui32>& hashes2);
ui64 EstimateJoinCardinality(const std::vector<ui32>& hashes1, const std::vector<ui32>& hashes2);
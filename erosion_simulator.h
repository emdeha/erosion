#ifndef EROSION_SIMULATOR_H
#define EROSION_SIMULATOR_H

#include <vector>
#include <cstdint>
#include <random>

class ErosionSimulator {
public:
    ErosionSimulator(uint32_t seed = 0);

    std::vector<std::vector<float>> erode(const std::vector<std::vector<float>>& heightMap, uint32_t iterations);

private:
    std::mt19937 m_rng;
    
    void erodePoint(std::vector<std::vector<float>>& heightMap, uint32_t x, uint32_t y);
    float getInterpolatedHeight(const std::vector<std::vector<float>>& heightMap, float x, float y);
};

#endif // EROSION_SIMULATOR_H

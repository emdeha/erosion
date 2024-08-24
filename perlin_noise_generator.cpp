#include "perlin_noise_generator.h"
#include <algorithm>
#include <cmath>

PerlinNoiseGenerator::PerlinNoiseGenerator(uint32_t seed, double frequency, int octaves)
    : m_perlinNoise(seed), m_frequency(frequency), m_octaves(octaves) {}

std::vector<std::vector<float>> PerlinNoiseGenerator::generate(uint32_t width, uint32_t height) {
    std::vector<std::vector<float>> heightMap(height, std::vector<float>(width, 0.0f));

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            double nx = static_cast<double>(x) / width - 0.5;
            double ny = static_cast<double>(y) / height - 0.5;
            
            double elevation = 0.0;
            double amplitude = 1.0;
            double frequency = m_frequency;
            double maxValue = 0.0;
            
            for (int o = 0; o < m_octaves; ++o) {
                double sampleX = nx * frequency;
                double sampleY = ny * frequency;
                elevation += m_perlinNoise.noise(sampleX, sampleY) * amplitude;
                
                maxValue += amplitude;
                amplitude *= 0.5;
                frequency *= 2.0;
            }
            
            elevation /= maxValue;
            elevation = (elevation + 1.0) / 2.0;  // Normalize to [0, 1]
            
            heightMap[y][x] = static_cast<float>(elevation);
        }
    }

    return heightMap;
}

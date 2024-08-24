#ifndef PERLIN_NOISE_GENERATOR_H
#define PERLIN_NOISE_GENERATOR_H

#include "terrain_generator.h"
#include "perlin_noise.h"

class PerlinNoiseGenerator : public TerrainGenerator {
public:
    PerlinNoiseGenerator(uint32_t seed = 0, double frequency = 0.1, int octaves = 4);
    std::vector<std::vector<float>> generate(uint32_t width, uint32_t height) override;

private:
    PerlinNoise m_perlinNoise;
    double m_frequency;
    int m_octaves;
};

#endif // PERLIN_NOISE_GENERATOR_H

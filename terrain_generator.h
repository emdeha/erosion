#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <vector>
#include <cstdint>

class TerrainGenerator {
public:
    virtual ~TerrainGenerator() = default;
    virtual std::vector<std::vector<float>> generate(uint32_t width, uint32_t height) = 0;
};

#endif // TERRAIN_GENERATOR_H

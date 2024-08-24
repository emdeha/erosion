#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <cstdint>

class Terrain {
public:
    Terrain(uint32_t width, uint32_t height);

    void generate();
    float getHeight(uint32_t x, uint32_t y) const;
    void setHeight(uint32_t x, uint32_t y, float height);

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }

private:
    uint32_t m_width;
    uint32_t m_height;
    std::vector<std::vector<float>> m_heightMap;
};

#endif // TERRAIN_H

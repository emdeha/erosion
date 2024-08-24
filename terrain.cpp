#include "terrain.h"
#include <random>

Terrain::Terrain(uint32_t width, uint32_t height)
    : m_width(width), m_height(height) {
    m_heightMap.resize(height, std::vector<float>(width, 0.0f));
}

void Terrain::generate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            m_heightMap[y][x] = dis(gen);
        }
    }
}

float Terrain::getHeight(uint32_t x, uint32_t y) const {
    return m_heightMap[y][x];
}

void Terrain::setHeight(uint32_t x, uint32_t y, float height) {
    m_heightMap[y][x] = height;
}

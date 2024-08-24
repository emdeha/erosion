#include "terrain.h"

Terrain::Terrain(uint32_t width, uint32_t height, std::unique_ptr<TerrainGenerator> generator)
    : m_width(width), m_height(height), m_generator(std::move(generator)) {
    m_heightMap.resize(height, std::vector<float>(width, 0.0f));
}

void Terrain::generate() {
    m_heightMap = m_generator->generate(m_width, m_height);
}

float Terrain::getHeight(uint32_t x, uint32_t y) const {
    return m_heightMap[y][x];
}

void Terrain::setHeight(uint32_t x, uint32_t y, float height) {
    m_heightMap[y][x] = height;
}

#include "erosion_simulator.h"
#include <cmath>
#include <algorithm>

ErosionSimulator::ErosionSimulator(uint32_t seed) : m_rng(seed) {}

std::vector<std::vector<float>> ErosionSimulator::erode(const std::vector<std::vector<float>>& inputHeightMap, uint32_t iterations) {
    std::vector<std::vector<float>> heightMap = inputHeightMap; // Create a copy to work on
    uint32_t width = heightMap[0].size();
    uint32_t height = heightMap.size();

    std::uniform_int_distribution<uint32_t> xDist(0, width - 1);
    std::uniform_int_distribution<uint32_t> yDist(0, height - 1);

    for (uint32_t i = 0; i < iterations; ++i) {
        uint32_t x = xDist(m_rng);
        uint32_t y = yDist(m_rng);
        erodePoint(heightMap, x, y);
    }

    return heightMap;
}

void ErosionSimulator::erodePoint(std::vector<std::vector<float>>& heightMap, uint32_t x, uint32_t y) {
    const float inertia = 0.05f;
    const float minSlope = 0.01f;
    const float capacity = 4.0f;
    const float deposition = 0.3f;
    const float erosion = 0.3f;

    float posX = static_cast<float>(x);
    float posY = static_cast<float>(y);
    float dirX = 0.0f;
    float dirY = 0.0f;
    float speed = 1.0f;
    float water = 1.0f;
    float sediment = 0.0f;

    uint32_t width = heightMap[0].size();
    uint32_t height = heightMap.size();

    while (water > 0.01f) {
        int cellX = static_cast<int>(posX);
        int cellY = static_cast<int>(posY);
        
        // Ensure we're within bounds
        if (cellX < 0 || cellX >= static_cast<int>(width) - 1 || cellY < 0 || cellY >= static_cast<int>(height) - 1) {
            break;
        }

        // Calculate gradient
        float gradX = (getInterpolatedHeight(heightMap, posX + 1, posY) - getInterpolatedHeight(heightMap, posX - 1, posY)) * 0.5f;
        float gradY = (getInterpolatedHeight(heightMap, posX, posY + 1) - getInterpolatedHeight(heightMap, posX, posY - 1)) * 0.5f;

        // Update direction
        dirX = (dirX * inertia - gradX * (1 - inertia));
        dirY = (dirY * inertia - gradY * (1 - inertia));
        
        // Normalize direction
        float len = std::sqrt(dirX * dirX + dirY * dirY);
        if (len != 0) {
            dirX /= len;
            dirY /= len;
        }

        // Update position
        posX += dirX;
        posY += dirY;

        // Stop if we're out of bounds
        if (posX < 0 || posX >= width - 1 || posY < 0 || posY >= height - 1) {
            break;
        }

        // Calculate height difference
        float newHeight = getInterpolatedHeight(heightMap, posX, posY);
        float deltaHeight = newHeight - heightMap[cellY][cellX];

        // Deposit or erode
        if (deltaHeight > 0 || speed < minSlope) {
            if (sediment > 0) {
                float amountToDeposit = std::min(deltaHeight, sediment);
                sediment -= amountToDeposit;
                heightMap[cellY][cellX] += amountToDeposit * deposition;
            }
        } else {
            float amountToErode = std::min(-deltaHeight, capacity * speed - sediment) * erosion;
            heightMap[cellY][cellX] -= amountToErode;
            sediment += amountToErode;
        }

        // Update speed and water
        speed = std::sqrt(speed * speed + deltaHeight);
        water *= 0.99f;
    }
}

float ErosionSimulator::getInterpolatedHeight(const std::vector<std::vector<float>>& heightMap, float x, float y) {
    int x0 = static_cast<int>(std::floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(std::floor(y));
    int y1 = y0 + 1;

    // Ensure we're within bounds
    uint32_t width = heightMap[0].size();
    uint32_t height = heightMap.size();
    x0 = std::clamp(x0, 0, static_cast<int>(width) - 1);
    x1 = std::clamp(x1, 0, static_cast<int>(width) - 1);
    y0 = std::clamp(y0, 0, static_cast<int>(height) - 1);
    y1 = std::clamp(y1, 0, static_cast<int>(height) - 1);

    float fx = x - x0;
    float fy = y - y0;

    float h00 = heightMap[y0][x0];
    float h10 = heightMap[y0][x1];
    float h01 = heightMap[y1][x0];
    float h11 = heightMap[y1][x1];

    float h0 = h00 * (1 - fx) + h10 * fx;
    float h1 = h01 * (1 - fx) + h11 * fx;

    return h0 * (1 - fy) + h1 * fy;
}

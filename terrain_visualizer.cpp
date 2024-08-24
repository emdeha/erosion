#include "terrain_visualizer.h"
#include <sstream>
#include <algorithm>

std::string TerrainVisualizer::visualizeASCII(const Terrain& terrain) {
    std::stringstream ss;
    const char* asciiChars = " .-:=+*#%@";
    const int numChars = 10;

    for (uint32_t y = 0; y < terrain.getHeight(); ++y) {
        for (uint32_t x = 0; x < terrain.getWidth(); ++x) {
            float height = terrain.getHeight(x, y);
            int index = std::min(static_cast<int>(height * numChars), numChars - 1);
            ss << asciiChars[index];
        }
        ss << '\n';
    }

    return ss.str();
}

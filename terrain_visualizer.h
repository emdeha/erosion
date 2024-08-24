#ifndef TERRAIN_VISUALIZER_H
#define TERRAIN_VISUALIZER_H

#include "terrain.h"
#include <string>

class TerrainVisualizer {
public:
    static std::string visualizeASCII(const Terrain& terrain);
};

#endif // TERRAIN_VISUALIZER_H

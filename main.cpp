#include <iostream>
#include "terrain.h"
#include "terrain_visualizer.h"

int main() {
  Terrain terrain(256, 256);
  terrain.generate();

  std::cout << "Terrain generated with size: " 
            << terrain.getWidth() << "x" << terrain.getHeight() << std::endl;

  std::string visualization = TerrainVisualizer::visualizeASCII(terrain);
  std::cout << "Terrain Visualization:\n" << visualization << std::endl;

  return 0;
}

#include <iostream>
#include <fstream>
#include <memory>
#include "terrain.h"
#include "terrain_visualizer.h"
#include "perlin_noise_generator.h"

int main() {
  uint32_t width = 150;
  uint32_t height = 50;
  uint32_t seed = 304994;  // You can change this seed to get different terrains

  auto generator = std::make_unique<PerlinNoiseGenerator>(seed, 1.2, 3);
  Terrain terrain(width, height, std::move(generator));
  terrain.generate();

  std::cout << "Terrain generated with size: " 
            << terrain.getWidth() << "x" << terrain.getHeight() << std::endl;

  // Visualize the terrain
  std::string visualization = TerrainVisualizer::visualizeASCII(terrain);
  std::cout << "Terrain Visualization:\n" << visualization << std::endl;

  return 0;
}

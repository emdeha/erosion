#include <iostream>
#include <fstream>
#include <memory>
#include "terrain.h"
#include "terrain_visualizer_2d.h"
#include "terrain_visualizer.h"
#include "perlin_noise_generator.h"
#include "erosion_simulator.h"

int main() {
    uint32_t width = 150;
    uint32_t height = 50;
    uint32_t seed = 12345;  // You can change this seed to get different terrains

    auto generator = std::make_unique<PerlinNoiseGenerator>(seed, 1.1, 4);
    auto erosionSimulator = std::make_unique<ErosionSimulator>(seed);
    Terrain terrain(width, height, std::move(generator), std::move(erosionSimulator));
    terrain.generate();

    try {
      TerrainVisualizer2D visualizer2D(800, 600);
      visualizer2D.visualize(terrain);
    } catch (const std::exception& e) {
      std::cerr << "Error in 2D visualization: " << e.what() << std::endl;
    }

    return 0;
}

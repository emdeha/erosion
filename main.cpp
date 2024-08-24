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

/*
    terrain.generate();
    std::cout << "Terrain generated with size: " 
              << terrain.getWidth() << "x" << terrain.getHeight() << std::endl;

    // Visualize the terrain before erosion
    std::string visualizationBefore = TerrainVisualizer::visualizeASCII(terrain);
    std::cout << "Terrain Visualization (Before Erosion):\n" << visualizationBefore << std::endl;

    // Apply erosion
    terrain.erode(1000);  // Adjust the number of iterations as needed

    // Visualize the terrain after erosion
    std::string visualizationAfter = TerrainVisualizer::visualizeASCII(terrain);
    std::cout << "Terrain Visualization (After Erosion):\n" << visualizationAfter << std::endl;
*/

    return 0;
}

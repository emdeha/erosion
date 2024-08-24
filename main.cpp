#include <iostream>
#include <memory>
#include "terrain.h"
#include "terrain_visualizer_2d.h"
#include "perlin_noise_generator.h"
#include "erosion_simulator.h"
#include "terrain_visualizer_3d.h"

int main() {
    uint32_t width = 200;
    uint32_t height = 150;
    uint32_t seed = 30449;  // You can change this seed to get different terrains

    auto generator = std::make_unique<PerlinNoiseGenerator>(seed, 2.1, 2);
    auto erosionSimulator = std::make_unique<ErosionSimulator>(seed);
    Terrain terrain(width, height, std::move(generator), std::move(erosionSimulator));

    terrain.generate();

    try {
        TerrainVisualizer3D visualizer3D(800, 600);

        auto erodeStep = [](Terrain& t) {
            t.erode(50);
        };

        int fps = 30;
        int totalSteps = 60 * fps;  // 60 seconds * 30 fps

        visualizer3D.animateErosion(terrain, erodeStep, totalSteps, fps);
    } catch (const std::exception& e) {
        std::cerr << "Error in 3D visualization: " << e.what() << std::endl;
    }

    return 0;
}

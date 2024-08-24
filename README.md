# Terrain Erosion Simulator

This project simulates terrain generation and erosion using Perlin noise and hydraulic erosion algorithms. It provides both 2D and 3D visualizations using SDL2 and OpenGL respectively.

## Setup Instructions

### Prerequisites

- CMake (version 3.20 or higher)
- C++20 compatible compiler
- SDL2
- GLEW
- GLFW
- GLM

### Installation

#### macOS (using Homebrew)

1. Install Homebrew if you haven't already:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. Install the required libraries:

```bash
brew install cmake sdl2 glew glfw glm
```

#### Other platforms

Please refer to the official documentation for installing the required libraries on your specific platform.

### Building the Project

1. Clone the repository:

```bash
git clone https://github.com/yourusername/terrain-erosion-simulator.git
cd terrain-erosion-simulator
```

2. Create a build directory and navigate into it:

```bash
mkdir build && cd build
```

3. Generate the build files with CMake:

```bash
cmake ..
```

4. Build the project:

```bash
make
```

## How to Run

After building the project, you can run the simulator using:

```bash
./TerrainGenerator
```

This will start the simulation and open a window displaying the terrain erosion process.

## Switching between SDL and OpenGL

The project supports both SDL (2D) and OpenGL (3D) visualizations. To switch between them, you need to modify the `main.cpp` file:

#### For SDL (2D) visualization:

```cpp
#include "terrain_visualizer_2d.h"

// In the main function:
TerrainVisualizer2D visualizer(800, 600);
visualizer.visualize(terrain);
```

### For OpenGL (3D) visualization:

```cpp
#include "terrain_visualizer_3d.h"

// In the main function:
TerrainVisualizer3D visualizer(1024, 768);
visualizer.visualize(terrain);
```

After making the changes, rebuild the project using the steps in the "Building the Project" section.

## Notes

* The erosion simulation parameters can be adjusted in the main.cpp file.
* The terrain generation settings (such as Perlin noise parameters) can be modified in the PerlinNoiseGenerator constructor call in main.cpp.
* For the 3D visualization, you can adjust the camera position and terrain scaling in the TerrainVisualizer3D class.

## Contributing

Feel free to fork this project and submit pull requests with improvements or bug fixes. For major changes, please open an issue first to discuss what you would like to change.

## License

MIT License

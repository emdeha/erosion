cmake_minimum_required(VERSION 3.30)
project(TerrainGenerator VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    # Use brew's SDL2 installation location
    set(SDL2_DIR "/opt/homebrew/lib/cmake/SDL2")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
else()
    # For other platforms, use the default method
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

add_executable(TerrainGenerator
  main.cpp
  terrain.cpp
  terrain_visualizer.cpp
  terrain_visualizer_2d.cpp
  perlin_noise.cpp
  perlin_noise_generator.cpp
  erosion_simulator.cpp
)

if(APPLE)
    target_link_libraries(TerrainGenerator PRIVATE SDL2::SDL2)
else()
    target_link_libraries(TerrainGenerator PRIVATE ${SDL2_LIBRARIES})
endif()

cmake_minimum_required(VERSION 3.30)
project(TerrainGenerator VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TerrainGenerator
  main.cpp
  terrain.cpp
  terrain_visualizer.cpp
  perlin_noise.cpp
  perlin_noise_generator.cpp
)

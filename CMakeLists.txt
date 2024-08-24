cmake_minimum_required(VERSION 3.20)
project(TerrainGenerator VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Homebrew prefix for Apple Silicon
set(HOMEBREW_PREFIX "/opt/homebrew")

# SDL2
set(SDL2_DIR "${HOMEBREW_PREFIX}/lib/cmake/SDL2")
find_package(SDL2 REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
set(GLEW_DIR "${HOMEBREW_PREFIX}/lib/cmake/glew")
find_package(GLEW REQUIRED)

# GLFW
set(glfw3_DIR "${HOMEBREW_PREFIX}/lib/cmake/glfw3")
find_package(glfw3 REQUIRED)

# GLM
set(GLM_DIR "${HOMEBREW_PREFIX}/lib/cmake/glm")
find_package(glm REQUIRED)

add_executable(TerrainGenerator
  main.cpp
  terrain.cpp
  terrain_visualizer.cpp
  terrain_visualizer_2d.cpp
  perlin_noise.cpp
  perlin_noise_generator.cpp
  erosion_simulator.cpp
  terrain_visualizer_3d.cpp
)

target_include_directories(TerrainGenerator PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${HOMEBREW_PREFIX}/include
)

target_link_directories(TerrainGenerator PRIVATE
  ${HOMEBREW_PREFIX}/lib
)

target_link_libraries(TerrainGenerator PRIVATE
  SDL2::SDL2
  OpenGL::GL
  GLEW::GLEW
  glfw
  glm::glm
  "-framework OpenGL"
)

# Add this line to print GLEW include directories
message(STATUS "GLEW include dirs: ${GLEW_INCLUDE_DIRS}")

#include <iostream>
#include "terrain.h"

int main() {
  Terrain terrain(256, 256);
  terrain.generate();

  std::cout << "Terrain generated with size: " 
            << terrain.getWidth() << "x" << terrain.getHeight() << std::endl;

  for (uint32_t y = 0; y < 5; ++y) {
      for (uint32_t x = 0; x < 5; ++x) {
          std::cout << terrain.getHeight(x, y) << " ";
      }
      std::cout << std::endl;
  }

  return 0;
}

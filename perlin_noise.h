#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>
#include <cstdint>

class PerlinNoise {
public:
    PerlinNoise(uint32_t seed = 0);
    double noise(double x, double y) const;

private:
    std::vector<int> p;
    
    double fade(double t) const;
    double lerp(double t, double a, double b) const;
    double grad(int hash, double x, double y) const;
};

#endif // PERLIN_NOISE_H

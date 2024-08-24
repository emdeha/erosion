#include "perlin_noise.h"
#include <algorithm>
#include <random>
#include <cmath>

PerlinNoise::PerlinNoise(uint32_t seed) {
    p.resize(256);
    std::iota(p.begin(), p.end(), 0);
    
    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.end(), engine);
    
    p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    
    x -= std::floor(x);
    y -= std::floor(y);
    
    double u = fade(x);
    double v = fade(y);
    
    int A = p[X] + Y;
    int B = p[X + 1] + Y;
    
    return lerp(v, lerp(u, grad(p[A], x, y),
                           grad(p[B], x - 1, y)),
                   lerp(u, grad(p[A + 1], x, y - 1),
                           grad(p[B + 1], x - 1, y - 1)));
}

double PerlinNoise::fade(double t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) const {
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y) const {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

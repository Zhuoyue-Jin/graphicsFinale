#ifndef PERLIN_H
#define PERLIN_H

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <random>

class Perlin {
public:
    static float noise(float x, float z);

private:
    static glm::vec2 gradient(int hash);
    static float fade(float t);
    static float lerp(float a, float b, float t);
    static float dotGridGradient(int ix, int iz, float x, float z);

    static std::vector<int> permutation;
};

// Precomputed permutation vector for gradients
std::vector<int> Perlin::permutation = []() {
    std::vector<int> p(256);
    std::iota(p.begin(), p.end(), 0);
    std::shuffle(p.begin(), p.end(), std::default_random_engine(42));
    p.insert(p.end(), p.begin(), p.end()); // Duplicate the permutation for wrapping
    return p;
}();

float Perlin::noise(float x, float z) {
    // Determine grid cell coordinates
    int x0 = static_cast<int>(std::floor(x)) & 255;
    int z0 = static_cast<int>(std::floor(z)) & 255;

    int x1 = (x0 + 1) & 255;
    int z1 = (z0 + 1) & 255;

    // Relative x, z within grid cell
    float dx = x - std::floor(x);
    float dz = z - std::floor(z);

    // Fade curves for dx, dz
    float u = fade(dx);
    float v = fade(dz);

    // Hash gradient indices for corners of the cell
    int g00 = permutation[permutation[x0] + z0];
    int g01 = permutation[permutation[x0] + z1];
    int g10 = permutation[permutation[x1] + z0];
    int g11 = permutation[permutation[x1] + z1];

    // Calculate gradients for corners
    float n00 = dotGridGradient(g00, dx, dz);
    float n01 = dotGridGradient(g01, dx, dz - 1);
    float n10 = dotGridGradient(g10, dx - 1, dz);
    float n11 = dotGridGradient(g11, dx - 1, dz - 1);

    // Interpolate along x
    float nx0 = lerp(n00, n10, u);
    float nx1 = lerp(n01, n11, u);

    // Interpolate along z
    return lerp(nx0, nx1, v);
}

glm::vec2 Perlin::gradient(int hash) {
    switch (hash & 3) {
    case 0: return glm::vec2(1, 0);
    case 1: return glm::vec2(0, 1);
    case 2: return glm::vec2(-1, 0);
    case 3: return glm::vec2(0, -1);
    }
    return glm::vec2(0, 0); // Should never happen
}

float Perlin::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10); // Smoothing curve
}

float Perlin::lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float Perlin::dotGridGradient(int hash, float x, float z) {
    glm::vec2 gradientVec = gradient(hash);
    return gradientVec.x * x + gradientVec.y * z;
}

#endif // PERLIN_H

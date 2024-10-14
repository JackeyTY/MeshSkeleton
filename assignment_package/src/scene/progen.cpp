#include "progen.h"
#include "noisehelper.h"

std::vector<int> ProGen::getBlockHeight(int x, int z) {
    glm::vec2 xz = glm::vec2(x, z);
    float grassHeight = getGrasslandHeight(xz);
    float mountainHeight = getMountainHeight(xz);
    float dessertHeight = getDessertHeight(xz);

    double perlinT = biomeBlender(xz / 480.f);
    float grassMountT = glm::smoothstep(0.53, 0.65, perlinT);
    float dessertT = glm::smoothstep(0.43, 0.46, perlinT);

    int curBlockHiehgt = glm::floor(glm::mix(dessertHeight, glm::mix(grassHeight, mountainHeight, grassMountT), dessertT));
    int blockType;
    if (perlinT <= 0.44) {
        blockType = 0; // dessert
    } else if (perlinT > 0.44 && perlinT < 0.45) {
        blockType = 1; // dessert blend grass
    } else if (perlinT >= 0.45 && perlinT <= 0.52) {
        blockType = 2; // grass
    } else if (perlinT > 0.52 && perlinT < 0.62) {
        blockType = 3; // grass blend mount
    } else {
        blockType = 4;
    }
    return {curBlockHiehgt, blockType};
}

float ProGen::biomeBlender(glm::vec2 xz) {
    return 0.5 * (PerlinNoise(xz) + 1.f);
}

float ProGen::getGrasslandHeight(glm::vec2 uv) {
    uv /= 512;
    glm::vec2 offset = glm::vec2(fbm(uv), fbm(glm::vec2(uv[1], uv[0])));
    uv += offset;
    float noise = glm::clamp(WorleyNoise(uv), 0.f, 1.f);
    float grassHeight = (160 - 128) * noise + 128;
    return grassHeight;
}

float ProGen::getMountainHeight(glm::vec2 uv) {
    float sum = 0;
    float amp = 0.5;
    float freq = 48;
    for (int i = 0; i < 5; i++) {
        sum += abs(PerlinNoise(uv / freq)) * amp;
        amp *= 1.12;
        freq *= 2.f;
    }
    float noise = glm::clamp(sum, 0.f, 1.f);
    float mountHeight = (254 - 160) * noise + 160;
    return mountHeight;
}

float ProGen::getDessertHeight(glm::vec2 uv) {
    float sum = 0;
    float amp = 0.5;
    float freq = 96;
    for(int i = 0; i < 8; ++i) {
        glm::vec2 offset = glm::vec2(fbm(uv / 256.f), fbm(uv / 300.f) + 1000);
        sum += PerlinNoise((uv + offset * 75.f) / freq) * amp;
        amp *= 0.5;
        freq *= 0.5;
    }
    float noise = glm::clamp((float)abs(glm::smoothstep(0.22, 0.24, (double) sum) * 0.8 + (0.2 * sum)), 0.f, 1.f);
    float dessertHeight = (159 - 120) * noise + 120;
    return dessertHeight;
}

bool ProGen::keepCave(int x, int y, int z) {
    float noise = PerlinNoise3D(glm::vec3(x, y, z) / 16.f);
    return noise >= -0.1f;
}

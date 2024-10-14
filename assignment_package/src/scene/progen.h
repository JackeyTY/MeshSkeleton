#pragma once
#include "glm_includes.h"
#include <vector>

class ProGen
{
public:
    std::vector<int> getBlockHeight(int x, int z);
    bool keepCave(int x, int y, int z);

private:
    float biomeBlender(glm::vec2 xz);
    float getGrasslandHeight(glm::vec2 uv);
    float getMountainHeight(glm::vec2 uv);
    float getDessertHeight(glm::vec2 uv);
};

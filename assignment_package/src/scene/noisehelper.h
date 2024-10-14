#pragma once
#include <glm_includes.h>
#include <cmath>

glm::vec2 smoothF(glm::vec2 uv) {
    return uv * uv * (3.f - 2.f * uv);
}

float noise(glm::vec2 uv) {
    const float k = 257.f;
    glm::vec4 l  = glm::vec4(glm::floor(uv), glm::fract(uv));
    float u = l.x + l.y * k;
    glm::vec4 v  = glm::vec4(u, u + 1.f, u + k, u + k+1.f);
    v = glm::fract(glm::fract(1.23456789f * v ) * v / 0.987654321f);
    glm::vec2 zw = glm::vec2(l.z, l.w);
    zw = smoothF(zw);
    l.x = glm::mix(v.x, v.y, zw[0]);
    l.y = glm::mix(v.z, v.w, zw[0]);
    return glm::mix(l.x, l.y, zw[1]);
}

float fbm(glm::vec2 uv) {
    float a = 0.5;
    float f = 5.0;
    float n = 0.f;
    for (int i = 0; i < 8; i++) {
        n += noise(uv * f) * a;
        a *= 0.5f;
        f *= 2.f;
    }
    return n;
}

glm::vec2 random2(glm::vec2 p) {
    return glm::fract(glm::sin(glm::vec2(glm::dot(p,glm::vec2(127.1,311.7)),glm::dot(p,glm::vec2(269.5,183.3)))) * 43758.5453f);
}

float surflet(glm::vec2 P, glm::vec2 gridPoint) {
    // Compute falloff function by converting linear distance to a polynomial (quintic smootherstep function)
    float distX = abs(P.x - gridPoint.x);
    float distY = abs(P.y - gridPoint.y);
    float tX = 1 - 6 * pow(distX, 5.0) + 15 * pow(distX, 4.0) - 10 * pow(distX, 3.0);
    float tY = 1 - 6 * pow(distY, 5.0) + 15 * pow(distY, 4.0) - 10 * pow(distY, 3.0);

    // Get the random vector for the grid point
    glm::vec2 gradient = 2.f * random2(gridPoint) - glm::vec2(1.f);
    // Get the vector from the grid point to P
    glm::vec2 diff = P - gridPoint;
    // Get the value of our height field by dotting grid->P with our gradient
    float height = glm::dot(diff, gradient);
    // Scale our height field (i.e. reduce it) by our polynomial falloff function
    return height * tX * tY;
}

float PerlinNoise(glm::vec2 uv) {
    // Tile the space
    glm::vec2 uvXLYL = glm::floor(uv);
    glm::vec2 uvXHYL = uvXLYL + glm::vec2(1,0);
    glm::vec2 uvXHYH = uvXLYL + glm::vec2(1,1);
    glm::vec2 uvXLYH = uvXLYL + glm::vec2(0,1);

    return surflet(uv, uvXLYL) + surflet(uv, uvXHYL) + surflet(uv, uvXHYH) + surflet(uv, uvXLYH);
}

glm::vec3 random3(glm::vec3 p) {
    float x = glm::fract(sin(glm::dot(p, glm::vec3(12.9898, 78.233, 45.543))) * 43758.5453);
    float y = glm::fract(sin(glm::dot(p, glm::vec3(21.9898, 34.456, 98.166))) * 43758.5453);
    float z = glm::fract(sin(glm::dot(p, glm::vec3(87.123, 45.789, 12.654))) * 43758.5453);
    return glm::vec3(x, y, z);
}

float surflet3D(glm::vec3 P, glm::vec3 gridPoint) {
    float distX = abs(P.x - gridPoint.x);
    float distY = abs(P.y - gridPoint.y);
    float distZ = abs(P.z - gridPoint.z);
    float tX = 1 - 6 * pow(distX, 5.0) + 15 * pow(distX, 4.0) - 10 * pow(distX, 3.0);
    float tY = 1 - 6 * pow(distY, 5.0) + 15 * pow(distY, 4.0) - 10 * pow(distY, 3.0);
    float tZ = 1 - 6 * pow(distZ, 5.0) + 15 * pow(distZ, 4.0) - 10 * pow(distZ, 3.0);

    glm::vec3 gradient = glm::normalize(2.f * random3(gridPoint) - glm::vec3(1.f));
    glm::vec3 diff = P - gridPoint;
    float height = glm::dot(diff, gradient);
    return height * tX * tY * tZ;
}

float PerlinNoise3D(glm::vec3 xyz) {
    float surfletSum = 0.f;
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            for (int dz = 0; dz < 2; dz++) {
                surfletSum += surflet3D(xyz, glm::floor(xyz) + glm::vec3(dx, dy, dz));
            }
        }
    }
    return surfletSum;
}

float WorleyNoise(glm::vec2 uv) {
    // Tile the space
    glm::vec2 uvInt = glm::floor(uv);
    glm::vec2 uvFract = glm::fract(uv);

    float minDist = 1.0; // Minimum distance initialized to max.
    float secondMinDist = 1.0;

    // Search all neighboring cells and this cell for their point
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            glm::vec2 neighbor = glm::vec2(float(x), float(y));

            // Random point inside current neighboring cell
            glm::vec2 point = random2(uvInt + neighbor);

            // Compute the distance b/t the point and the fragment
            // Store the min dist thus far
            glm::vec2 diff = neighbor + point - uvFract;
            float dist = glm::length(diff);
            if(dist < minDist) {
                secondMinDist = minDist;
                minDist = dist;
            }
            else if(dist < secondMinDist) {
                secondMinDist = dist;
            }
        }
    }
    return secondMinDist - minDist;
}

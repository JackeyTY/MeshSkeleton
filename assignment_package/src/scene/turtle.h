#ifndef TURTLE_H
#define TURTLE_H
#pragma once
#include "glm_includes.h"

class Turtle
{
public:
    Turtle(int, int, float);
    int posx;
    int posz;
    float orientation;
};
#endif // TURTLE_H

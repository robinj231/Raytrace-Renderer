#pragma once
#include "vec3.h"

class cLight
{
public:
    cLight();
    cLight(bool pointLight, cVec3 vec3, cVec3 color);

    bool pointLight;
    cVec3 vec3;
    cVec3 color;
};
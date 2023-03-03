
#pragma once
#include "vec3.h"

class cRay
{
public:
    cVec3 pos;
    cVec3 dir;

    cRay(cVec3 pos, cVec3 dir)
    {
        this->pos = pos;
        this->dir = dir;
    }
};
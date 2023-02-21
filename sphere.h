#pragma once
#include "vec3.h"

class cPrimitive
{
public:
    cVec3 centerPos;
    int materialIndex;
    virtual cVec3 getNormal(cVec3 point) = 0;
};

class cSphere : public cPrimitive
{
public:
    float radius;
    cSphere(cVec3 centerPos, float radius, int materialIndex);
    cVec3 getNormal(cVec3 point) override;
};

class cCylinder : public cPrimitive
{
public:
    cVec3 upVector;
    float radius;
    float halfHeight;
    cCylinder(cVec3 centerPos, cVec3 upVector, float radius, float halfHeight, int materialIndex);
    cVec3 getNormal(cVec3 point) override;
};
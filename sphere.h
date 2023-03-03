#pragma once
#include "vec3.h"
#include "ray.h"
#include <vector>

class cPrimitive
{
public:
    cVec3 centerPos;
    int materialIndex;
    virtual cVec3 getNormal(cVec3 point) = 0;
    virtual float rayCollision(cRay ray) = 0;
};

class cSphere : public cPrimitive
{
public:
    float radius;
    cSphere(cVec3 centerPos, float radius, int materialIndex);
    cVec3 getNormal(cVec3 point) override;
    float rayCollision(cRay ray) override;
};

class cCylinder : public cPrimitive
{
public:
    cVec3 upVector;
    float radius;
    float halfHeight;
    cCylinder(cVec3 centerPos, cVec3 upVector, float radius, float halfHeight, int materialIndex);
    cVec3 getNormal(cVec3 point) override;
    //float rayCollision(cRay ray) override;
};

class cTriangle : public cPrimitive
{
public:
    int point0Index;
    int point1Index;
    int point2Index;
    int normal0Index;
    int normal1Index;
    int normal2Index;
    cTriangle(int point0Index, int point1Index, int point2Index, int materialIndex);
    cVec3 getNormal(cVec3 point) override;
    float rayCollision(cRay ray) override;
};
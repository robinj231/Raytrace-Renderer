#include "sphere.h"

cSphere::cSphere(cVec3 centerPos, float radius, int materialIndex)
{
    this->centerPos = centerPos;
    this->radius = radius;
    this->materialIndex = materialIndex;
}

cVec3 cSphere::getNormal(cVec3 point)
{
    return cVec3::normalized(cVec3::scale(cVec3::sub(point, centerPos), 1.0/radius));
}

cCylinder::cCylinder(cVec3 centerPos, cVec3 upVector, float radius, float halfHeight, int materialIndex)
{
    this->centerPos = centerPos;
    this->upVector = upVector;
    this->radius = radius;
    this->halfHeight = halfHeight;
    this->materialIndex = materialIndex;
}

cVec3 cCylinder::getNormal(cVec3 point)
{
    return cVec3::normalized(cVec3::scale(cVec3::sub(point, centerPos), 1.0/radius));
}
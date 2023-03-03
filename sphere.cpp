#include "sphere.h"

extern std::vector<cVec3> vertexArray;
extern std::vector<cVec3> normalArray;

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

float cSphere::rayCollision(cRay ray)
{
    cVec3 vA = ray.pos;
    cVec3 vB = cVec3::add(ray.pos, ray.dir);
    cVec3 vC = centerPos;

    float a = pow((vB.x-vA.x),2) + pow((vB.y-vA.y),2) + pow((vB.z-vA.z),2);
    float b = 2*((vB.x-vA.x)*(vA.x-vC.x) + (vB.y-vA.y)*(vA.y-vC.y) + (vB.z-vA.z)*(vA.z-vC.z));
    float c = pow((vA.x-vC.x),2) + pow((vA.y-vC.y),2) + pow((vA.z-vC.z),2) - pow(radius, 2);
    float delta = pow(b, 2) - 4*a*c;
    
    if(delta < 0)
    {
        return -1;
    }
    else if(delta > 0)
    {
        float d1 = (-b-sqrt(delta))/2*a;
        float d2 = (-b+sqrt(delta))/2*a;
        
        float closest = -1;
        if(d1 > 0 && d2 <= 0)
        {
            closest = d1;
        }
        else if(d1 <= 0 && d2 > 0)
        {
            closest = d2;
        }
        else if(d1 > 0 && d2 > 0)
        {
            closest = std::min(d1, d2);
        }

        return closest;
    }
    else // delta == 0
    {
        return -b/2*a > 0;
    }
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

cTriangle::cTriangle(int point0Index, int point1Index, int point2Index, int materialIndex)
{
    this->point0Index = point0Index;
    this->point1Index = point1Index;
    this->point2Index = point2Index;
    this->materialIndex = materialIndex;
}

cVec3 cTriangle::getNormal(cVec3 point)
{
    return cVec3(0,0,1);
}

float cTriangle::rayCollision(cRay ray)
{
    cVec3 ptZeroToOne = cVec3::sub(vertexArray.at(point1Index), vertexArray.at(point0Index));
    cVec3 ptZeroToTwo = cVec3::sub(vertexArray.at(point2Index), vertexArray.at(point0Index));
    cVec3 planeNormal = cVec3::cross(ptZeroToOne, ptZeroToTwo);

    // Plane Equation, A*x + B*y + C*z + D = 0
    float d = -(planeNormal.x * vertexArray.at(point0Index).x + planeNormal.y * vertexArray.at(point0Index).y + planeNormal.z * vertexArray.at(point0Index).z);

    float denominator = planeNormal.x * ray.dir.x + planeNormal.y * ray.dir.y + planeNormal.z * ray.dir.z;

    if(denominator != 0)
    {
        return -(planeNormal.x * ray.pos.x + planeNormal.y * ray.pos.y + planeNormal.z * ray.pos.z + d)/denominator;
    }
    else
    {
        return -1;
    }
}
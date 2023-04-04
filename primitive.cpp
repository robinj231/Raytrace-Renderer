#include "primitive.h"
#include <iostream>

extern std::vector<cVec3> vertexArray;
extern std::vector<cVec3> normalArray;
extern std::vector<std::vector<std::vector<cVec3>>> textures;
extern std::vector<std::pair<float,float>> texCoordArray;

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

cVec3 cSphere::getTexCoords(cVec3 point)
{
    float phi = acos((point.z - centerPos.z)/radius);
    float theta = atan2(point.y - centerPos.y, point.x - centerPos.x);

    float u;
    if(theta >= 0)
    {
        u = theta /(2*3.141592653);
    }
    else
    {
        u = (theta+2*3.141592653) /(2*3.141592653);
    }
    float v = phi/3.141592653;


    int vIndex = round(v*(textures[textureIndex].size()-1));
    vIndex %= textures[textureIndex].size()-1;

    if(vIndex < 0)
    {
        vIndex = (textures[textureIndex].size()-1) + vIndex;
    }

    int uIndex = round(u*(textures[textureIndex][0].size()-1));
    uIndex %= textures[textureIndex][0].size()-1;

    if(uIndex < 0)
    {
        uIndex = (textures[textureIndex][0].size()-1) + uIndex;
    }

    return textures[textureIndex][vIndex][uIndex];
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

cVec3 cCylinder::getTexCoords(cVec3 point)
{
    return cVec3(0,0,0);
}

cTriangle::cTriangle(int point0Index, int point1Index, int point2Index, int materialIndex)
{
    this->point0Index = point0Index;
    this->point1Index = point1Index;
    this->point2Index = point2Index;
    this->materialIndex = materialIndex;
    smoothShading = false;
}

cVec3 cTriangle::getNormal(cVec3 point)
{
    if(smoothShading)
    {
        cVec3 e1 = cVec3::sub(vertexArray[point1Index], vertexArray[point0Index]);
        cVec3 e2 = cVec3::sub(vertexArray[point2Index], vertexArray[point0Index]);
        cVec3 ep = cVec3::sub(point, vertexArray[point0Index]);

        float d11 = cVec3::dot(e1, e1);
        float d22 = cVec3::dot(e2, e2);
        float d12 = cVec3::dot(e1, e2);
        float d1p = cVec3::dot(e1, ep);
        float d2p = cVec3::dot(e2, ep);
        
        float determinant = d11*d22 - d12*d12;

        float beta = (d22*d1p - d12*d2p)/determinant;
        float gamma = (d11*d2p - d12*d1p)/determinant;
        float alpha = 1.0 - beta - gamma;

        return cVec3::normalized(cVec3::add(cVec3::scale(normalArray[normal0Index], alpha),
            cVec3::add(cVec3::scale(normalArray[normal1Index], beta), cVec3::scale(normalArray[normal2Index], gamma))));
    }
    else
    {
        cVec3 e1 = cVec3::sub(vertexArray[point1Index], vertexArray[point0Index]);
        cVec3 e2 = cVec3::sub(vertexArray[point2Index], vertexArray[point0Index]);
    
        return cVec3::normalized(cVec3::cross(e1, e2));
    }
}

float cTriangle::rayCollision(cRay ray)
{
    cVec3 ptZeroToOne = cVec3::sub(vertexArray.at(point1Index), vertexArray.at(point0Index));
    cVec3 ptZeroToTwo = cVec3::sub(vertexArray.at(point2Index), vertexArray.at(point0Index));
    cVec3 planeNormal = cVec3::cross(ptZeroToOne, ptZeroToTwo);

    if(cVec3::dot(ray.dir, planeNormal) > 0)
    {
        return -1;
    }

    // Plane Equation, A*x + B*y + C*z + D = 0
    float d = -(planeNormal.x * vertexArray.at(point0Index).x + planeNormal.y * vertexArray.at(point0Index).y + planeNormal.z * vertexArray.at(point0Index).z);

    float denominator = planeNormal.x * ray.dir.x + planeNormal.y * ray.dir.y + planeNormal.z * ray.dir.z;

    float collisionDist = -1;

    if(denominator != 0)
    {
        collisionDist = -(planeNormal.x * ray.pos.x + planeNormal.y * ray.pos.y + planeNormal.z * ray.pos.z + d)/denominator;
    }
    else
    {
        return -1;
    }

    // triangle bounds test
    cVec3 e1 = cVec3::sub(vertexArray[point1Index], vertexArray[point0Index]);
    cVec3 e2 = cVec3::sub(vertexArray[point2Index], vertexArray[point0Index]);
    cVec3 ep = cVec3::sub(cVec3::add(ray.pos, cVec3::scale(ray.dir, collisionDist)), vertexArray[point0Index]);

    float d11 = cVec3::dot(e1, e1);
    float d22 = cVec3::dot(e2, e2);
    float d12 = cVec3::dot(e1, e2);
    float d1p = cVec3::dot(e1, ep);
    float d2p = cVec3::dot(e2, ep);

    float determinant = d11*d22 - d12*d12;

    if(determinant != 0)
    {
        float beta = (d22*d1p - d12*d2p)/determinant;
        float gamma = (d11*d2p - d12*d1p)/determinant;

        if(beta > 0 && gamma > 0 && beta + gamma < 1)
        {
            return collisionDist;
        }
    }

    return -1;
}

cVec3 cTriangle::getTexCoords(cVec3 point)
{
    cVec3 e1 = cVec3::sub(vertexArray[point1Index], vertexArray[point0Index]);
    cVec3 e2 = cVec3::sub(vertexArray[point2Index], vertexArray[point0Index]);
    cVec3 ep = cVec3::sub(point, vertexArray[point0Index]);

    float d11 = cVec3::dot(e1, e1);
    float d22 = cVec3::dot(e2, e2);
    float d12 = cVec3::dot(e1, e2);
    float d1p = cVec3::dot(e1, ep);
    float d2p = cVec3::dot(e2, ep);
    
    float determinant = d11*d22 - d12*d12;

    float beta = (d22*d1p - d12*d2p)/determinant;
    float gamma = (d11*d2p - d12*d1p)/determinant;
    float alpha = 1.0 - beta - gamma;

    float u = alpha*texCoordArray[texCoord0Index].first + beta*texCoordArray[texCoord1Index].first + gamma*texCoordArray[texCoord2Index].first;
    float v = alpha*texCoordArray[texCoord0Index].second + beta*texCoordArray[texCoord1Index].second + gamma*texCoordArray[texCoord2Index].second;



    int vIndex = round(v*(textures[textureIndex].size()-1));
    vIndex %= textures[textureIndex].size()-1;

    if(vIndex < 0)
    {
        vIndex = (textures[textureIndex].size()-1) + vIndex;
    }

    int uIndex = round(u*(textures[textureIndex][0].size()-1));
    uIndex %= textures[textureIndex][0].size()-1;

    if(uIndex < 0)
    {
        uIndex = (textures[textureIndex][0].size()-1) + uIndex;
    }

    return textures[textureIndex][vIndex][uIndex];
}
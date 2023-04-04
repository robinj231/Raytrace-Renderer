#include "vec3.h"
#include <iostream>

cVec3::cVec3()
{
    x = y = z = 0;
}

cVec3::cVec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

cVec3 cVec3::normalized(cVec3 v)
{
    return scale(v, 1.0/v.len());
}

cVec3 cVec3::scale(cVec3 v, float scalar)
{
    return cVec3(v.x*scalar, v.y*scalar, v.z*scalar);
}

float cVec3::len()
{
    return std::sqrt(dot(*this,*this));
}

cVec3 cVec3::add(cVec3 v1, cVec3 v2)
{
    return cVec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// Remember: Resultant vector will always point toward v1's end
cVec3 cVec3::sub(cVec3 v1, cVec3 v2)
{
    return cVec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

float cVec3::dot(cVec3 v1, cVec3 v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

cVec3 cVec3::cross(cVec3 v1, cVec3 v2)
{
    return cVec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

float cVec3::angle(cVec3 v1, cVec3 v2)
{
    return std::acos(dot(normalized(v1), normalized(v2)
    ));
}

std::string cVec3::toString()
{
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}

cVec3 cVec3::rotate(cVec3 v, cVec3 axis, float angle)
{
    float qX = std::sin(angle/2.0)*axis.x;
    float qY = std::sin(angle/2.0)*axis.y;
    float qZ = std::sin(angle/2.0)*axis.z;
    float qW = std::cos(angle/2.0);

    float v0x = qW*v.x + /*qX*0 +*/ + qY*v.z - qZ*v.y;
    float v0y = qW*v.y - qX*v.z + /*qY*0 +*/ qZ*v.x;
    float v0z = qW*v.z + qX*v.y - qY*v.x /*+ qZ*0*/;
    float v0w = /*qW*0*/ - qX*v.x - qY*v.y - qZ*v.z;

    float v1x = v0w*-qX + v0x*qW + v0y*-qZ - v0z*-qY;
    float v1y = v0w*-qY - v0x*-qZ + v0y*qW + v0z*-qX;
    float v1z = v0w*-qZ + v0x*-qY - v0y*-qX + v0z*qW;
    //float v1w = v0w*qW - v0x*-qX - v0y*-qY - v0z*-qZ;

    return cVec3(v1x, v1y, v1z);
}

float cVec3::r()
{
    return x;
}

float cVec3::g()
{
    return y;
}

float cVec3::b()
{
    return z;
}

void cVec3::setR(float r)
{
    x = r;
}

void cVec3::setG(float g)
{
    y = g;
}

void cVec3::setB(float b)
{
    z = b;
}
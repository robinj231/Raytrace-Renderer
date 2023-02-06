#include "vec3.h"
#include <iostream>

vec3::vec3()
{
    x = y = z = 0;
}

vec3::vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3 vec3::normalized(vec3 v)
{
    return scale(v, 1.0/v.len());
}

vec3 vec3::scale(vec3 v, float scalar)
{
    return vec3(v.x*scalar, v.y*scalar, v.z*scalar);
}

float vec3::len()
{
    return std::sqrt(dot(*this,*this));
}

vec3 vec3::add(vec3 v1, vec3 v2)
{
    return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// Remember: Resultant vector will always point toward v1's end
vec3 vec3::sub(vec3 v1, vec3 v2)
{
    return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

float vec3::dot(vec3 v1, vec3 v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

vec3 vec3::cross(vec3 v1, vec3 v2)
{
    return vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

float vec3::angle(vec3 v1, vec3 v2)
{
    return std::acos(dot(normalized(v1), normalized(v2)
    ));
}

std::string vec3::toString()
{
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "\n";
}

vec3 vec3::rotate(vec3 v, vec3 axis, float angle)
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

    return vec3(v1x, v1y, v1z);
}
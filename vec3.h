#pragma once
#include <string>
#include <cmath>

class cVec3
{
public:
    cVec3();
    cVec3(float x, float y, float z);

    float x;
    float y;
    float z;

    static cVec3 normalized(cVec3 v);
    static cVec3 scale(cVec3 v, float scalar);
    float len();

    static cVec3 add(cVec3 v1, cVec3 v2);
    static cVec3 sub(cVec3 v1, cVec3 v2);

    static float dot(cVec3 v1, cVec3 v2);
    static cVec3 cross(cVec3 v1, cVec3 v2);

    static float angle(cVec3 v1, cVec3 v2);

    std::string toString();

    static cVec3 rotate(cVec3 v, cVec3 axis, float angle);

    // to make things more readable when using as a color

    float r();
    float g();
    float b();
    void setR(float r);
    void setG(float g);
    void setB(float b);
};
#pragma once
#include <string>
#include <cmath>

class vec3
{
public:
    vec3();
    vec3(float x, float y, float z);

    float x;
    float y;
    float z;

    static vec3 normalized(vec3 v);
    static vec3 scale(vec3 v, float scalar);
    float len();

    static vec3 add(vec3 v1, vec3 v2);
    static vec3 sub(vec3 v1, vec3 v2);

    static float dot(vec3 v1, vec3 v2);
    static vec3 cross(vec3 v1, vec3 v2);

    static float angle(vec3 v1, vec3 v2);

    std::string toString();

    static vec3 rotate(vec3 v, vec3 axis, float angle);
};
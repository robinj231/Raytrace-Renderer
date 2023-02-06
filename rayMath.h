#pragma once
#include "vec3.h"
#include <iostream>
#include <vector>

float degreesToRadians(float degrees)
{
    return degrees * 0.0174532925;
}

float radiansToDegrees(float radians)
{
    return radians * 57.2957795;
}

struct ColorType
{
    int r = 0;
    int b = 0;
    int g = 0;

    ColorType(int r, int b, int g)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

struct RayType
{
    vec3 pos;
    vec3 dir;

    RayType(vec3 pos, vec3 dir)
    {
        this->pos = pos;
        this->dir = dir;
    }
};

struct SphereType
{
    vec3 pos;
    float radius;
    int materialIndex;

    SphereType(vec3 pos, float radius, int materialIndex)
    {
        this->pos = pos;
        this->radius = radius;
        this->materialIndex = materialIndex;
    }
};


bool RaySphereCollision(RayType ray, SphereType sphere)
{
    vec3 vA = ray.pos;
    vec3 vB = vec3::add(ray.pos, ray.dir);
    vec3 vC = sphere.pos;

    float a = pow((vB.x-vA.x),2) + pow((vB.y-vA.y),2) + pow((vB.z-vA.z),2);
    float b = 2*((vB.x-vA.x)*(vA.x-vC.x) + (vB.y-vA.y)*(vA.y-vC.y) + (vB.z-vA.z)*(vA.z-vC.z));
    float c = pow((vA.x-vC.x),2) + pow((vA.y-vC.y),2) + pow((vA.z-vC.z),2) - pow(sphere.radius, 2);
    float delta = pow(b, 2) - 4*a*c;
    
    if(delta < 0)
    {
        return false;
    }
    else if(delta > 0)
    {
        float d1 = (-b-sqrt(delta))/2*a;
        float d2 = (-b+sqrt(delta))/2*a;
        
        // will need to check signage and relative values when calculating lighting
        return (d1 > 0 || d2 > 0);
    }
    else // delta == 0
    {
        return -b/2*a > 0;
    }
}


int TraceRay( RayType ray, std::vector<SphereType> env, ColorType bgColor )
/* “ray” specifies the incident ray (origin and
direction); for assignment 1a it is the ray
from the eye through a point in the view window */
/* please be forewarned that this parameter list will
evolve; later on we will need to pass in
more information */
{ /* for each object in the scene, check for a
ray/object intersection; keep track of the
closest intersection point to the eye and the
identity of the object hit at that point */
/* if a valid intersection is found, call Shade_Ray()
to determine the pixel color; otherwise, return the
background color */

for(int s = 0; s < env.size(); s++)
{
    if(RaySphereCollision(ray, env.at(s)))
    {
        return env.at(s).materialIndex;
    }
}

return 0;
}
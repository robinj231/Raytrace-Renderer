#pragma once
#include "vec3.h"
#include "sphere.h"
#include "light.h"
#include <iostream>
#include <vector>
#include <cmath>

float degreesToRadians(float degrees)
{
    return degrees * 0.0174532925;
}

float radiansToDegrees(float radians)
{
    return radians * 57.2957795;
}

class cMaterial
{
public:
    cVec3 color;
    cVec3 specColor;
    float ambientMult;
    float diffuseMult;
    float specMult;
    float specExponent;

    cMaterial(cVec3 color, float ambientMult, float diffuseMult, float specMult, float specExponent)
    {
        this->color = color;
        specColor = cVec3(1,1,1);
        this->ambientMult = ambientMult;
        this->diffuseMult = diffuseMult;
        this->specMult = specMult;
        this->specExponent = specExponent;
    }  
    cMaterial(cVec3 color, cVec3 specColor, float ambientMult, float diffuseMult, float specMult, float specExponent)
    {
        this->color = color;
        this->specColor = specColor;
        this->ambientMult = ambientMult;
        this->diffuseMult = diffuseMult;
        this->specMult = specMult;
        this->specExponent = specExponent;
    }
};

class cRay
{
public:
    cVec3 pos;
    cVec3 dir;

    cRay(cVec3 pos, cVec3 dir)
    {
        this->pos = pos;
        this->dir = dir;
    }
};

struct sHitData
{
    cVec3 collisionPt;
    int objIndex;
    float distance;
};

std::vector<cMaterial> materials;
std::vector<cSphere> spheres;
std::vector<cLight> lightEnv;

cVec3 camPos = cVec3(0,0,0);
cVec3 camDir = cVec3(0,0,1);
cVec3 camUp = cVec3(0,1,0);

cVec3 u, v, w;

cVec3 ul, ur, ll, lr;

float hFOV = 120;
float vFOV = 90;

cVec3 deltaH, deltaV;

float viewDist = 1;

float windowWidth;
float windowHeight;

int imgWidth = 100;
int imgHeight = 100;

float aspectRatio;

cVec3 bgColor = cVec3(0,0,0);

float RaySphereCollision(cRay ray, cSphere sphere)
{
    cVec3 vA = ray.pos;
    cVec3 vB = cVec3::add(ray.pos, ray.dir);
    cVec3 vC = sphere.centerPos;

    float a = pow((vB.x-vA.x),2) + pow((vB.y-vA.y),2) + pow((vB.z-vA.z),2);
    float b = 2*((vB.x-vA.x)*(vA.x-vC.x) + (vB.y-vA.y)*(vA.y-vC.y) + (vB.z-vA.z)*(vA.z-vC.z));
    float c = pow((vA.x-vC.x),2) + pow((vA.y-vC.y),2) + pow((vA.z-vC.z),2) - pow(sphere.radius, 2);
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

sHitData* TraceRay(cRay ray, int self = -1)
{ 
    sHitData* hitData = NULL;
    float closestDist = INFINITY;

    for(int s = 0; s < spheres.size(); s++)
    {
        if(s != self)
        {
            float objDist = RaySphereCollision(ray, spheres.at(s));
            if(objDist > 0 && objDist < closestDist)
            {
                if(hitData == NULL)
                {
                    hitData = new sHitData();
                }
                hitData->distance = closestDist = objDist;
                hitData->objIndex = s;
            }
        }
    }

    if(hitData != NULL)
    {
        hitData->collisionPt = cVec3::add(ray.pos, cVec3::scale(ray.dir, hitData->distance));
    }

    return hitData;
}

cVec3 PhongShade(sHitData* hitData)
{
    cMaterial material = materials.at(spheres.at(hitData->objIndex).materialIndex);
    cVec3 pos = hitData->collisionPt;
    cVec3 normal = spheres.at(hitData->objIndex).getNormal(hitData->collisionPt);

    cVec3 returnColor = cVec3::scale(material.color, material.ambientMult);

    cVec3 camVector = cVec3::normalized(cVec3::sub(camPos, pos));
    cVec3 lVector;
    cVec3 hVector;

    cVec3 diffuseComponent;
    cVec3 specComponent;
    bool noShadowFlag;
    for(int l = 0; l < lightEnv.size(); l++)
    {
        noShadowFlag = true;
        if(!lightEnv.at(l).pointLight)
        {
            lVector = cVec3::scale(cVec3::normalized(lightEnv.at(l).vec3), -1);
            noShadowFlag = (TraceRay(cRay(pos, lVector), hitData->objIndex) == NULL);
        }
        else
        {
            lVector = cVec3::normalized(cVec3::sub(lightEnv.at(l).vec3, pos));
            sHitData* shadowHitData = TraceRay(cRay(pos, lVector), hitData->objIndex);
            noShadowFlag = (shadowHitData == NULL || shadowHitData->distance > cVec3::sub(lightEnv.at(l).vec3, pos).len());
        }
        hVector = cVec3::normalized(cVec3::add(lVector, camVector));

        diffuseComponent = cVec3::scale(material.color, std::max((float)0, cVec3::dot(normal, lVector)*material.diffuseMult));
        specComponent = cVec3::scale(material.specColor, std::pow(std::max((float)0, cVec3::dot(normal, hVector)), material.specExponent)*material.specMult);
        cVec3 diffuseAndSpec = cVec3::add(diffuseComponent, specComponent);
        returnColor = cVec3::add(returnColor, cVec3::scale(cVec3(diffuseAndSpec.r()*lightEnv.at(l).color.r(), diffuseAndSpec.g()*lightEnv.at(l).color.g(), diffuseAndSpec.b()*lightEnv.at(l).color.b()), noShadowFlag));
    }

    returnColor.setR(std::min((float)1, returnColor.r()));
    returnColor.setG(std::min((float)1, returnColor.g()));
    returnColor.setB(std::min((float)1, returnColor.b()));
    return returnColor;
}

cVec3 GetFragmentColor(cRay ray)
{
    cVec3 returnColor = bgColor;
    sHitData* hitData = TraceRay(ray);

    if(hitData != NULL)
    {
        returnColor = PhongShade(hitData);
    }

    return returnColor;
}
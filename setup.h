#pragma once
#include "vec3.h"
#include "ray.h"
#include "primitive.h"
#include "light.h"
#include <iostream>
#include <vector>
#include <cmath>

std::vector<cVec3> vertexArray;
std::vector<cVec3> normalArray;
std::vector<std::pair<float,float>> texCoordArray;

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

struct sHitData
{
    cVec3 collisionPt;
    int objIndex;
    float distance;
};

std::vector<std::vector<std::vector<cVec3>>> textures;
std::vector<cMaterial> materials;
std::vector<cPrimitive*> objects;
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

sHitData* TraceRay(cRay ray, int self = -1)
{ 
    sHitData* hitData = NULL;
    float closestDist = INFINITY;

    for(int s = 0; s < objects.size(); s++)
    {
        if(s != self)
        {
            float objDist = objects.at(s)->rayCollision(ray);
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
    cMaterial material = materials.at(objects.at(hitData->objIndex)->materialIndex);
    cVec3 pos = hitData->collisionPt;
    cVec3 normal = objects.at(hitData->objIndex)->getNormal(hitData->collisionPt);

    cVec3 diffuseColor;
    
    if(objects[hitData->objIndex]->textureIndex >= 0)
    {
        diffuseColor = objects[hitData->objIndex]->getTexCoords(hitData->collisionPt);
    }
    else
    {
        diffuseColor = material.color;
    }

    cVec3 camVector = cVec3::normalized(cVec3::sub(camPos, pos));
    cVec3 lVector;
    cVec3 hVector;
    cVec3 returnColor = cVec3::scale(diffuseColor, material.ambientMult);

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

        

        diffuseComponent = cVec3::scale(diffuseColor, std::max((float)0, cVec3::dot(normal, lVector)*material.diffuseMult));

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
#pragma once
#include "vec3.h"
#include "ray.h"
#include "primitive.h"
#include "light.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <stack>

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
    float opacity;
    float refractionIndex;

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
    cVec3 origin;
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

std::pair<cVec3, float> bgColor = {cVec3(0,0,0), 0};

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
        hitData->origin = ray.pos;
    }

    return hitData;
}

// used for light traveling through transparent objects
cVec3 ComputeRefraction(cVec3 normal, cVec3 iVector, float incomingIndex, float transmittingIndex)
{
    return cVec3::normalized(cVec3::add(cVec3::scale(cVec3::scale(normal, -1), sqrt(1 - (pow(incomingIndex / transmittingIndex, 2) * (1 - pow(cVec3::dot(normal, iVector), 2))))),
        cVec3::scale(cVec3::sub(cVec3::scale(normal, cVec3::dot(normal, iVector)), iVector), incomingIndex / transmittingIndex)));
}

cVec3 ShadeFragment(cRay ray, int self = -1, int reflectIter = 10, int refractIter = 10, bool entering = true)
{
    cVec3 returnColor = bgColor.first;

    if(reflectIter <= 0)
    {
        return cVec3(0,0,0);
    }

    sHitData* hitData = TraceRay(ray, self);
    if(hitData == NULL)
    {
        return returnColor;
    }

    cMaterial material = materials.at(objects.at(hitData->objIndex)->materialIndex);
    cVec3 pos = hitData->collisionPt;
    cVec3 normal = objects.at(hitData->objIndex)->getNormal(hitData->collisionPt);

    if(!entering)
    {
        normal = cVec3::normalized(cVec3::scale(normal, -1));
    }

    cVec3 diffuseColor;
    
    if(objects[hitData->objIndex]->textureIndex >= 0)
    {
        diffuseColor = objects[hitData->objIndex]->getTexCoords(hitData->collisionPt);
    }
    else
    {
        diffuseColor = material.color;
    }

    float opacity = materials[objects[hitData->objIndex]->materialIndex].opacity;

    cVec3 iVector = cVec3::scale(cVec3::normalized(ray.dir), -1);
    cVec3 lVector;
    cVec3 hVector;
    returnColor = cVec3::scale(diffuseColor, material.ambientMult);

    cVec3 diffuseComponent;
    cVec3 specComponent;
    float noShadowFlag;

    for(int l = 0; l < lightEnv.size(); l++)
    {
        noShadowFlag = 1;
        if(!lightEnv.at(l).pointLight)
        {
            lVector = cVec3::scale(cVec3::normalized(lightEnv.at(l).vec3), -1);
            sHitData* shadowHitData = TraceRay(cRay(pos, lVector), hitData->objIndex);
            float hitObjOpacity = 0;
            while(!(shadowHitData == NULL) && hitObjOpacity != 1)
            {
                hitObjOpacity = materials[objects[shadowHitData->objIndex]->materialIndex].opacity;
                noShadowFlag -= hitObjOpacity;
                shadowHitData = TraceRay(cRay(shadowHitData->collisionPt, lVector), shadowHitData->objIndex);
            }
        }
        else
        {
            lVector = cVec3::normalized(cVec3::sub(lightEnv.at(l).vec3, pos));
            sHitData* shadowHitData = TraceRay(cRay(pos, lVector), hitData->objIndex);
            float currentObjOpacity = 0;
            while(!(shadowHitData == NULL || shadowHitData->distance > cVec3::sub(lightEnv.at(l).vec3, pos).len()) && currentObjOpacity != 1)
            {
                currentObjOpacity = materials[objects[shadowHitData->objIndex]->materialIndex].opacity;
                noShadowFlag -= currentObjOpacity;
                shadowHitData = TraceRay(cRay(shadowHitData->collisionPt, lVector), shadowHitData->objIndex);
            }
        }

        if(noShadowFlag > 1) noShadowFlag = 1;
        else if(noShadowFlag < 0) noShadowFlag = 0;

        hVector = cVec3::normalized(cVec3::add(lVector, iVector));

        diffuseComponent = cVec3::scale(diffuseColor, std::max((float)0, cVec3::dot(normal, lVector)*material.diffuseMult));

        specComponent = cVec3::scale(material.specColor, std::pow(std::max((float)0, cVec3::dot(normal, hVector)), material.specExponent)*material.specMult);
        cVec3 diffuseAndSpec = cVec3::add(diffuseComponent, specComponent);
        returnColor = cVec3::add(returnColor, cVec3::scale(cVec3(diffuseAndSpec.r()*lightEnv.at(l).color.r(), diffuseAndSpec.g()*lightEnv.at(l).color.g(), diffuseAndSpec.b()*lightEnv.at(l).color.b()), noShadowFlag));
    }


    // first reflection section
    float incomingIndex;
    float transmittingIndex;

    if(entering)
    {
        incomingIndex = bgColor.second;
        transmittingIndex = materials[objects[hitData->objIndex]->materialIndex].refractionIndex;
    }
    else
    {
        incomingIndex = materials[objects[hitData->objIndex]->materialIndex].refractionIndex;
        transmittingIndex = bgColor.second;
    }

    cVec3 reflectedColor = cVec3(0,0,0);

    float fresnelZero = pow((transmittingIndex - incomingIndex)/(transmittingIndex + incomingIndex), 2);

    float fresnelReflect = fresnelZero + (1 - fresnelZero) * pow(1 - cVec3::dot(iVector, normal), 5);

    cVec3 nextStartPoint = cVec3::add(pos, cVec3::scale(normal, .1));

    if(materials[objects[hitData->objIndex]->materialIndex].specMult > 0)
    {
        // getting reflection vector and fresnel reflectance
        cVec3 reflectionVector = cVec3::normalized(cVec3::sub(cVec3::scale(normal, 2*cVec3::dot(normal, iVector)), iVector));

        if(abs(fresnelReflect) > 0.001)
        {
            reflectedColor = ShadeFragment(cRay(nextStartPoint, reflectionVector), -1, reflectIter-1, refractIter, entering);
        }
    }

    //reflectedColor = TraceReflectionRay(pos, materials[objects[hitData->objIndex]->materialIndex].specMult, fresnelReflect, iVector, normal, hitData->objIndex, reflectIter, refractIter);
    // end first reflection section

    cVec3 behindColor = bgColor.first;

    if(opacity < 1 && refractIter > 0)
    {
        cVec3 refractedVector = ComputeRefraction(normal, iVector, incomingIndex, transmittingIndex);
        //std::cout << "Entering object: " << refractedVector.toString() << std::endl;

        nextStartPoint = cVec3::add(pos, cVec3::scale(normal, -.1));

        behindColor = ShadeFragment(cRay(nextStartPoint, refractedVector), -1, reflectIter, refractIter-1, !entering);
    }
    
    returnColor = cVec3::add(returnColor, cVec3::scale(behindColor, (1.0 - opacity) * (1.0 - fresnelReflect)));
    returnColor = cVec3::add(returnColor, cVec3::scale(reflectedColor, fresnelReflect));

    returnColor.setR(std::min((float)1, returnColor.r()));
    returnColor.setG(std::min((float)1, returnColor.g()));
    returnColor.setB(std::min((float)1, returnColor.b()));

    return returnColor;
}
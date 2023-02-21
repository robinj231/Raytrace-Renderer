#include "light.h"

cLight::cLight()
{
    this->pointLight = false;
    vec3 = cVec3(0,0,0);
    color = cVec3(1,1,1);
}

cLight::cLight(bool pointLight, cVec3 vec3, cVec3 color)
{
    this->pointLight = pointLight;
    this->vec3 = vec3;
    this->color = color;
}
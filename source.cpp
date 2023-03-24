#include "vec3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include "light.h"
#include <array>


#include "setup.h"

cVec3 storeImage(int height, int width)
{
    return cVec3(0,0,0);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Input a scene file as first argument.";
        return 0;
    }

    std::ifstream inputFile (argv[1]);

    if(inputFile.is_open())
    {
        while(inputFile.good())
        {
            try
            {
                std::string string;
                inputFile >> string;
                std::cout << string << std::endl;
                if(string == "eye")
                {
                    inputFile >> string;
                    camPos.x = std::stof(string);
                    inputFile >> string;
                    camPos.y = std::stof(string);
                    inputFile >> string;
                    camPos.z = std::stof(string);
                }
                else if(string == "viewdir")
                {
                    inputFile >> string;
                    camDir.x = std::stof(string);
                    inputFile >> string;
                    camDir.y = std::stof(string);
                    inputFile >> string;
                    camDir.z = std::stof(string);
                    camDir = cVec3::normalized(camDir);
                }
                else if(string == "updir")
                {
                    inputFile >> string;
                    camUp.x = std::stof(string);
                    inputFile >> string;
                    camUp.y = std::stof(string);
                    inputFile >> string;
                    camUp.z = std::stof(string);
                    camUp = cVec3::normalized(camUp);
                }
                else if(string == "hfov")
                {
                    inputFile >> string;
                    hFOV = std::stoi(string);
                }
                else if(string == "imsize")
                {
                    inputFile >> string;
                    imgWidth = std::stoi(string);
                    inputFile >> string;
                    imgHeight = std::stoi(string);
                }
                else if(string == "bkgcolor")
                {
                    bgColor = cVec3(0,0,0);
                    inputFile >> string;
                    bgColor.setR(std::stof(string));
                    inputFile >> string;
                    bgColor.setG(std::stof(string));
                    inputFile >> string;
                    bgColor.setB(std::stof(string));
                }
                else if(string == "light")
                {
                    cLight light = cLight();
                    inputFile >> string;
                    light.vec3.x = std::stof(string);
                    inputFile >> string;
                    light.vec3.y = std::stof(string);
                    inputFile >> string;
                    light.vec3.z = std::stof(string);
                    inputFile >> string;
                    light.pointLight = std::stoi(string);
                    if(!light.pointLight)
                    {
                        light.vec3 = cVec3::normalized(light.vec3);
                    }
                    inputFile >> string;
                    light.color.setR(std::stof(string));
                    inputFile >> string;
                    light.color.setG(std::stof(string));
                    inputFile >> string;
                    light.color.setB(std::stof(string));
                    lightEnv.push_back(light);
                }
                else if(string == "mtlcolor")
                {
                    cMaterial material = cMaterial(cVec3(0,0,0),1,1,1,1);
                    inputFile >> string;
                    material.color.setR(std::stof(string));
                    inputFile >> string;
                    material.color.setG(std::stof(string));
                    inputFile >> string;
                    material.color.setB(std::stof(string));
                    inputFile >> string;
                    material.specColor.setR(std::stof(string));
                    inputFile >> string;
                    material.specColor.setG(std::stof(string));
                    inputFile >> string;
                    material.specColor.setB(std::stof(string));
                    inputFile >> string;
                    material.ambientMult = std::stof(string);
                    inputFile >> string;
                    material.diffuseMult = std::stof(string);
                    inputFile >> string;
                    material.specMult = std::stof(string);
                    inputFile >> string;
                    material.specExponent = std::stof(string);
                    materials.push_back(material);
                }
                else if(string == "sphere")
                {
                    cSphere* sphere = new cSphere(cVec3(0,0,0), 0, materials.size()-1);
                    inputFile >> string;
                    sphere->centerPos.x = std::stof(string);
                    inputFile >> string;
                    sphere->centerPos.y = std::stof(string);
                    inputFile >> string;
                    sphere->centerPos.z = std::stof(string);
                    inputFile >> string;
                    sphere->radius = std::stof(string);

                    if(textures.size() > 0)
                    {
                        sphere->textureIndex = textures.size()-1;
                    }
                    objects.push_back(sphere);
                }
                // add vertex into vertex array
                else if(string == "v")
                {
                    cVec3 newVertex = cVec3(0,0,0);
                    inputFile >> string;
                    newVertex.x = std::stof(string);
                    inputFile >> string;
                    newVertex.y = std::stof(string);
                    inputFile >> string;
                    newVertex.z = std::stof(string);
                    vertexArray.push_back(newVertex);
                }
                // add normal into normal array
                else if(string == "vn")
                {
                    cVec3 newNormal = cVec3(0,0,0);
                    inputFile >> string;
                    newNormal.x = std::stof(string);
                    inputFile >> string;
                    newNormal.y = std::stof(string);
                    inputFile >> string;
                    newNormal.z = std::stof(string);
                    normalArray.push_back(cVec3::normalized(newNormal));
                }
                // add uv coordinates to texCoord array
                else if(string == "vt")
                {
                    std::pair<float, float> newTexCoord;
                    inputFile >> string;
                    newTexCoord.first = std::stof(string);
                    inputFile >> string;
                    newTexCoord.second = std::stof(string);
                    texCoordArray.push_back(newTexCoord);
                }
                // add triangle
                else if(string == "f")
                {
                    cTriangle* triangle = new cTriangle(0,0,0, materials.size()-1);

                    inputFile >> string;
                    if(string.find("//") != std::string::npos)
                    {
                        triangle->smoothShading = true;

                        triangle->point0Index = std::stoi(string.substr(0, string.find("//")))-1;

                        triangle->normal0Index = std::stoi(string.substr(string.find("//")+2, string.length()))-1;
                        
                        inputFile >> string;

                        triangle->point1Index = std::stoi(string.substr(0, string.find("//")))-1;

                        triangle->normal1Index = std::stoi(string.substr(string.find("//")+2, string.length()))-1;

                        inputFile >> string;

                        triangle->point2Index = std::stoi(string.substr(0, string.find("//")))-1;

                        triangle->normal2Index = std::stoi(string.substr(string.find("//")+2, string.length()))-1;
                    }
                    else if(string.find("/") != std::string::npos)
                    {
                        int foundIndex = string.find("/");
                        if(string.find("/", foundIndex+1) != std::string::npos)
                        {
                            triangle->smoothShading = true;

                            triangle->point0Index = std::stoi(string.substr(0, foundIndex))-1;

                            triangle->texCoord0Index = std::stoi(string.substr(foundIndex+1, string.find("/", foundIndex+1)-foundIndex))-1;

                            triangle->normal0Index = std::stoi(string.substr(string.find("/", foundIndex+1)+1))-1;
                            
                            inputFile >> string;
                            foundIndex = string.find("/");

                            triangle->point1Index = std::stoi(string.substr(0, foundIndex))-1;

                            triangle->texCoord1Index = std::stoi(string.substr(foundIndex+1, string.find("/", foundIndex+1)-foundIndex))-1;

                            triangle->normal1Index = std::stoi(string.substr(string.find("/", foundIndex+1)+1))-1;

                            inputFile >> string;
                            foundIndex = string.find("/");

                            triangle->point2Index = std::stoi(string.substr(0, foundIndex))-1;

                            triangle->texCoord2Index = std::stoi(string.substr(foundIndex+1, string.find("/", foundIndex+1)-foundIndex))-1;

                            triangle->normal2Index = std::stoi(string.substr(string.find("/", foundIndex+1)+1))-1;
                        }
                        else
                        {
                            triangle->point0Index = std::stoi(string.substr(0, string.find("/")))-1;

                            triangle->texCoord0Index = std::stoi(string.substr(string.find("/")+1, string.length()))-1;
                            
                            inputFile >> string;

                            triangle->point1Index = std::stoi(string.substr(0, string.find("/")))-1;

                            triangle->texCoord1Index = std::stoi(string.substr(string.find("/")+1, string.length()))-1;

                            inputFile >> string;

                            triangle->point2Index = std::stoi(string.substr(0, string.find("/")))-1;

                            triangle->texCoord2Index = std::stoi(string.substr(string.find("/")+1, string.length()))-1;
                        }
                    }
                    else
                    {
                        triangle->point0Index = std::stoi(string)-1;
                        inputFile >> string;
                        triangle->point1Index = std::stoi(string)-1;
                        inputFile >> string;
                        triangle->point2Index = std::stoi(string)-1;
                    }

                    if(textures.size() > 0)
                    {
                        triangle->textureIndex = textures.size()-1;
                    }
                    objects.push_back(triangle);
                }
                else if(string == "texture")
                {
                    inputFile >> string;
                    std::ifstream textureFile (string);
                    int height;
                    int width;

                    // P3 header
                    textureFile >> string;

                    textureFile >> width;
                    textureFile >> height;

                    // max color val
                    textureFile >> string;

                    std::vector<std::vector<cVec3>> texture;

                    for(int j = 0; j < height; j++)
                    {
                        std::vector<cVec3> newRow;
                        texture.push_back(newRow);

                        for(int i = 0; i < width; i++)
                        {
                            cVec3 fragColor = cVec3(0,0,0);
                            textureFile >> string;
                            fragColor.setR(std::stof(string)/255);
                            textureFile >> string;
                            fragColor.setG(std::stof(string)/255);
                            textureFile >> string;
                            fragColor.setB(std::stof(string)/255);
                            texture.at(j).push_back(fragColor);
                        }
                    }

                    textures.push_back(texture);
                    textureFile.close();
                }
            }
            catch(...)
            {
                std::cout << "Error in number of arguments or in variable/argument name, check input file" << std::endl;
                return 0;
            }
        }

        inputFile.close();
    }

    aspectRatio = (float)imgWidth/imgHeight;
    hFOV = degreesToRadians(hFOV);
    vFOV = 2*atan(tan(hFOV/2)/aspectRatio);

    w = cVec3::normalized(cVec3::scale(camDir, -1));
    u = cVec3::normalized(cVec3::cross(camDir, camUp));
    v = cVec3::normalized(cVec3::cross(u, camDir));

    windowWidth = 2*viewDist*tan(hFOV/2);
    windowHeight = windowWidth / aspectRatio;

    ul = cVec3::add(cVec3::add(camPos, cVec3::scale(camDir, viewDist)), cVec3::add(cVec3::scale(u, -windowWidth/2), cVec3::scale(v, windowHeight/2)));
    ur = cVec3::add(cVec3::add(camPos, cVec3::scale(camDir, viewDist)), cVec3::add(cVec3::scale(u, windowWidth/2), cVec3::scale(v, windowHeight/2)));
    ll = cVec3::add(cVec3::add(camPos, cVec3::scale(camDir, viewDist)), cVec3::add(cVec3::scale(u, -windowWidth/2), cVec3::scale(v, -windowHeight/2)));
    lr = cVec3::add(cVec3::add(camPos, cVec3::scale(camDir, viewDist)), cVec3::add(cVec3::scale(u, windowWidth/2), cVec3::scale(v, -windowHeight/2)));

    deltaH = cVec3::scale(cVec3::sub(ur, ul), 1.0/(imgWidth-1));
    deltaV = cVec3::scale(cVec3::sub(ll, ul), 1.0/(imgHeight-1));

    std::string fileName = std::string(argv[1]);
    std::string outfileName = std::string(fileName).substr(0, fileName.length()-4) + ".ppm";
    std::ofstream outfile (outfileName);
    outfile << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

    for(int j = 0; j < imgHeight; j++)
    {
        for(int i = 0; i < imgWidth; i++)
        {
            cVec3 pixelWorldSpace = cVec3::add(ul, cVec3::add(cVec3::scale(deltaH, i), cVec3::scale(deltaV, j)));
            cRay ray = cRay(camPos, cVec3::normalized(cVec3::sub(pixelWorldSpace, camPos)));

            cVec3 color = GetFragmentColor(ray);
            
            outfile << std::floor(color.r()*255) << " " << std::floor(color.g()*255) << " " << std::floor(color.b()*255) << "\n";
        }
    }
    outfile.close();

    return 0;
};
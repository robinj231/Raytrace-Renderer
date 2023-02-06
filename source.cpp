#include "vec3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include "rayMath.h"


ColorType bgColor = ColorType(0,0,0);

vec3 camPos = vec3(0,0,0);
vec3 camDir = vec3(0,0,1);
vec3 camUp = vec3(0,1,0);

vec3 u, v, w;

vec3 ul, ur, ll, lr;

float hFOV = 120;
float vFOV = 90;

vec3 deltaH, deltaV;

float viewDist = 1;

float windowWidth;
float windowHeight;

int imgWidth = 100;
int imgHeight = 100;

float aspectRatio;

std::vector<SphereType> spheres;

std::vector<ColorType> materials;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Input a scene file as first argument and output file name as second";
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
                }
                else if(string == "updir")
                {
                    inputFile >> string;
                    camUp.x = std::stof(string);
                    inputFile >> string;
                    camUp.y = std::stof(string);
                    inputFile >> string;
                    camUp.z = std::stof(string);
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
                    ColorType bgColor = ColorType(0,0,0);
                    inputFile >> string;
                    bgColor.r = std::stof(string)*255;
                    inputFile >> string;
                    bgColor.g = std::stof(string)*255;
                    inputFile >> string;
                    bgColor.b = std::stof(string)*255;
                    materials.push_back(bgColor);
                }
                else if(string == "mtlcolor")
                {
                    ColorType mColor = ColorType(0,0,0);
                    inputFile >> string;
                    mColor.r = std::stof(string)*255;
                    inputFile >> string;
                    mColor.g = std::stof(string)*255;
                    inputFile >> string;
                    mColor.b = std::stof(string)*255;
                    materials.push_back(mColor);
                }
                else if(string == "sphere")
                {
                    SphereType sphere = SphereType(vec3(0,0,0), 0, materials.size()-1);
                    inputFile >> string;
                    sphere.pos.x = std::stof(string);
                    inputFile >> string;
                    sphere.pos.y = std::stof(string);
                    inputFile >> string;
                    sphere.pos.z = std::stof(string);
                    inputFile >> string;
                    sphere.radius = std::stof(string);
                    spheres.push_back(sphere);
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

    w = vec3::normalized(vec3::scale(camDir, -1));
    u = vec3::normalized(vec3::cross(camDir, camUp));
    v = vec3::normalized(vec3::cross(u, camDir));

    windowWidth = 2*viewDist*tan(hFOV/2);
    windowHeight = windowWidth / aspectRatio;

    ul = vec3::add(vec3::add(camPos, vec3::scale(camDir, viewDist)), vec3::add(vec3::scale(u, -windowWidth/2), vec3::scale(v, windowHeight/2)));
    ur = vec3::add(vec3::add(camPos, vec3::scale(camDir, viewDist)), vec3::add(vec3::scale(u, windowWidth/2), vec3::scale(v, windowHeight/2)));
    ll = vec3::add(vec3::add(camPos, vec3::scale(camDir, viewDist)), vec3::add(vec3::scale(u, -windowWidth/2), vec3::scale(v, -windowHeight/2)));
    lr = vec3::add(vec3::add(camPos, vec3::scale(camDir, viewDist)), vec3::add(vec3::scale(u, windowWidth/2), vec3::scale(v, -windowHeight/2)));

    deltaH = vec3::scale(vec3::sub(ur, ul), 1.0/(imgWidth-1));
    deltaV = vec3::scale(vec3::sub(ll, ul), 1.0/(imgHeight-1));

    std::string outfileName = std::string(argv[2]) + ".ppm";
    std::ofstream outfile (outfileName);
    outfile << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

    for(int j = 0; j < imgHeight; j++)
    {
        for(int i = 0; i < imgWidth; i++)
        {

            vec3 pixelWorldSpace = vec3::add(ul, vec3::add(vec3::scale(deltaH, i), vec3::scale(deltaV, j)));
            RayType ray = RayType(camPos, vec3::normalized(vec3::sub(pixelWorldSpace, camPos)));

            int colorIndex = TraceRay(ray, spheres, bgColor);
            ColorType color = materials.at(colorIndex);
            
            outfile << color.r << " " << color.g << " " << color.b << "\n";
        }
    }
    outfile.close();

    return 0;
};
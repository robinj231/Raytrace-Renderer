#include "vec3.h"
#include <iostream>
#include <fstream>
#include <math.h>

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

    SphereType(vec3 pos, float radius)
    {
        this->pos = pos;
        this->radius = radius;
    }
};

ColorType bgColor = ColorType(0,0,0);

vec3 camPos = vec3(0,0,0);
vec3 camDir = vec3(0,0,1);
vec3 camUp = vec3(0,1,0);

vec3 u;
vec3 v;
vec3 w;

float hFOV = 90;
float vFOV = 90;

float viewDist = 1;

float windowWidth;
float windowHeight;

int imgWidth = 10;
int imgHeight = 10;

int main()
{
    /* read scene description from input file */
    /* initialize pixel array for output image */
    /* perform any required preliminary calculations */
    /* for each pixel in the image array: */

    w = vec3::normalized(vec3::scale(camDir, -1));
    u = vec3::normalized(vec3::cross(w, camUp));
    v = vec3::normalized(vec3::cross(camDir, u));

    windowWidth = 2*viewDist*tan(hFOV/2);
    windowHeight = windowWidth / /*aspect ratio*/imgWidth/imgHeight;

    std::cout << u.toString() << v.toString() << vec3::angle(u, v) * 180/3.14;

    std::ofstream outfile ("img.ppm");
    outfile << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";

    for(int y = 0; y < imgHeight; y++)
    {
        for(int x = 0; x < imgWidth; x++)
        {
            int R, G, B;
            outfile << R << " " << G << " " << B << "\n";
        }
    }
    outfile.close();

    /* call Trace_Ray() with appropriate parameters */
    /* use the value returned by Trace_Ray() to define
    the pixel color */
    /* write the final image to an output file */

    return 0;
};



// ColorType Trace_Ray( RayType ray )
// /* “ray” specifies the incident ray (origin and
// direction); for assignment 1a it is the ray
// from the eye through a point in the view window */
// /* please be forewarned that this parameter list will
// evolve; later on we will need to pass in
// more information */
// { /* for each object in the scene, check for a
// ray/object intersection; keep track of the
// closest intersection point to the eye and the
// identity of the object hit at that point */
// /* if a valid intersection is found, call Shade_Ray()
// to determine the pixel color; otherwise, return the
// background color */
// return;
// }

// ColorType Shade_Ray(/* parameter list*/ )
// /* for assignment 1a, we only need to pass in an
// identifier that allows access to the material
// properties of the intersected object; in the future,
// we will need to also provide the coordinates of the
// point where the shading will be computed */
// {
// /* compute the color at the intersection point; presently
// this consists of just returning the sphere’s material
// color. Later, the illumination calculations will get
// more complicated, and you will eventually make
// recursive calls to TraceRay from within Shade_Ray when
// your program is extended */
// return bgColor;
// }
#include "vec3.h"
#include <iostream>
#include <math.h>

int main()
{
    /* read scene description from input file */
    /* initialize pixel array for output image */
    /* perform any required preliminary calculations */
    /* for each pixel in the image array: */
    /* call Trace_Ray() with appropriate parameters */
    /* use the value returned by Trace_Ray() to define
    the pixel color */
    /* write the final image to an output file */

    return 0;
};

ColorType Trace_Ray( RayType ray )
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
return( return_color );
}

ColorType Shade_Ray( parameter list )
/* for assignment 1a, we only need to pass in an
identifier that allows access to the material
properties of the intersected object; in the future,
we will need to also provide the coordinates of the
point where the shading will be computed */
{
/* compute the color at the intersection point; presently
this consists of just returning the sphere’s material
color. Later, the illumination calculations will get
more complicated, and you will eventually make
recursive calls to TraceRay from within Shade_Ray when
your program is extended */
return( computed_color );
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

    SphereType(vec3 pos, float radius)
    {
        this->pos = pos;
        this->radius = radius;
    }
};
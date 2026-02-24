#ifndef SCENE_H
#define SCENE_H

#include "util.h"
#include "light.h"
#include <string.h>
#include <stdint.h>

typedef struct Sphere
{
    float radius;
    Point3D center;
    uint32_t color;
    float specular;
} Sphere;

typedef struct Viewport {
    float width;
    float height;
    float distance;
} Viewport;

typedef struct Scene
{
    Viewport viewport;
    int sphere_count;
    Sphere *spheres;
    int light_count;
    Light *lights;
} Scene;

Scene* initializeScene(Sphere[], int, Light[], int);

#endif

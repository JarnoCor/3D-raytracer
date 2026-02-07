#ifndef SCENE_H
#define SCENE_H

#include "util.h"
#include <string.h>
#include <stdint.h>

typedef struct Sphere
{
    float radius;
    Point3D center;
    uint32_t color;
} Sphere;

typedef struct Viewport {
    int width;
    int height;
    float distance;
} Viewport;
typedef struct Scene
{
    Viewport viewport;
    int sphere_count;
    Sphere spheres[];
} Scene;

Scene* initializeScene(int count, Sphere* sphere);

#endif

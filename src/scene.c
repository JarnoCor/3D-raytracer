#include "scene.h"

Scene* initializeScene(int count, Sphere spheres[])
{
    Scene *scene = (Scene*) malloc(sizeof(Scene) + count * sizeof(Sphere));
    scene->sphere_count = count;
    memcpy(scene->spheres, spheres, count * sizeof(Sphere));

    Viewport viewport = { 1, 1, 1 };
    scene->viewport = viewport;

    return scene;
}
#include "scene.h"

Scene* initializeScene(Sphere spheres[], int sphere_count, Light lights[], int light_count)
{
    Scene *scene = (Scene*) malloc(sizeof(Scene));

    scene->sphere_count = sphere_count;
    scene->spheres = malloc(sphere_count * sizeof(Sphere));
    memcpy(scene->spheres, spheres, sphere_count * sizeof(Sphere));

    scene->light_count = light_count;
    scene->lights = malloc(light_count * sizeof(Light));
    memcpy(scene->lights, lights, light_count * sizeof(Light));

    Viewport viewport = { 1, 1, 1 };
    scene->viewport = viewport;

    return scene;
}

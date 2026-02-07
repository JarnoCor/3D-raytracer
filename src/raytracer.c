#include "raytracer.h"
#include <stdio.h>

Point3D canvasToViewport(Canvas* canvas, Viewport viewport, int x, int y)
{
    return (Point3D) { 1.0f*x*viewport.width/canvas->width, 1.0f*y*viewport.height/canvas->height, viewport.distance };
}

void intersectRaySphere(float* t1, float* t2, Point3D origin, Point3D viewport, Sphere sphere)
{
    float r = sphere.radius;
    Point3D vector_origin_sphere = (Point3D) { origin.x - sphere.center.x, origin.y - sphere.center.y, origin.z - sphere.center.z };

    float a = dotProduct(viewport, viewport);
    float b = 2*dotProduct(vector_origin_sphere, viewport);
    float c = dotProduct(vector_origin_sphere, vector_origin_sphere) - r*r;

    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        *t1 = INFINITY;
        *t2 = INFINITY;
        return;
    }

    *t1 = (-b + sqrtf(discriminant)) / (2*a);
    *t2 = (-b - sqrtf(discriminant)) / (2*a);
}

uint32_t traceRay(Canvas* canvas, Point3D origin, Point3D viewport, float t_min, float t_max)
{
    float closest_t = INFINITY;
    Sphere *closest_sphere = 0;

    float t1, t2 = 0;
    for (int i = 0; i < canvas->scene->sphere_count; i++)
    {
        Sphere *current_sphere = &canvas->scene->spheres[i];
        intersectRaySphere(&t1, &t2, origin, viewport, *current_sphere);

        if (t1 >= t_min && t1 <= t_max && t1 < closest_t)
        {
            closest_t = t1;
            closest_sphere = current_sphere;
        }
        if (t2 >= t_min && t2 <= t_max && t2 < closest_t)
        {
            closest_t = t2;
            closest_sphere = current_sphere;
        }
    }

    if (!closest_sphere)
    {
        return canvas->background_color;
    }

    char buffer[256];
    wchar_t wbuffer[256];

    sprintf(buffer, "X position: %.6f", closest_sphere->center.x);

    // Converteer char buffer naar wchar_t buffer
    mbstowcs(wbuffer, buffer, sizeof(wbuffer) / sizeof(wchar_t));

    // Gebruik de Unicode-versie
    OutputDebugStringW(wbuffer);

    return closest_sphere->color;
}

void render(Canvas* canvas)
{
    Point3D origin = { 0, 0, 0 };

    for (int x = -canvas->width/2; x < canvas->width/2; x++)
    {
        for (int y = canvas->height/2; y > -canvas->height/2; y--)
        {
            Point3D viewport_point = canvasToViewport(canvas, canvas->scene->viewport, x, y);
            uint32_t color = traceRay(canvas, origin, viewport_point, 1, INFINITY);
            setPixel(canvas, x, y, color);
        }
    }
}

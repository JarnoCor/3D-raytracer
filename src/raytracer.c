#include "raytracer.h"
#include <stdio.h>

Point3D canvasToViewport(Canvas* canvas, Viewport viewport, int x, int y)
{
    return (Point3D) { 1.0f*x*viewport.width/canvas->width, 1.0f*y*viewport.height/canvas->height, viewport.distance };
}

void intersectRaySphere(float* t1, float* t2, Point3D starting_point, Vec3 direction, Sphere sphere)
{
    float r = sphere.radius;
    Vec3 vector_origin_sphere = (Vec3) { starting_point.x - sphere.center.x, starting_point.y - sphere.center.y, starting_point.z - sphere.center.z };

    float a = dotProduct(direction, direction);
    float b = 2*dotProduct(vector_origin_sphere, direction);
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

float computeLighting(Canvas* canvas, Point3D point, Vec3 normal, Vec3 viewport, float specular)
{

    float intensity = 0.0f;

    for (int i = 0; i < canvas->scene->light_count; i++)
    {
        Light *current_light = &canvas->scene->lights[i];

        if (current_light->type == LIGHT_AMBIENT)
        {
            intensity += current_light->intensity;
        } else
        {
            Vec3 l;
            if (current_light->type == LIGHT_POINT)
            {
                l = (Vec3) { current_light->data.point.position.x - point.x, current_light->data.point.position.y - point.y, current_light->data.point.position.z - point.z };
            } else
            {
                l = current_light->data.directional.direction;
            }

            // diffuse
            float n_dot_l = dotProduct(normal, l);
            if (n_dot_l > 0)
            {
                intensity += current_light->intensity * n_dot_l/(lengthVector(normal) * lengthVector(l));
            }

            // specular
            if (specular != -1)
            {
                Vec3 r = (Vec3) { 2 * normal.x * n_dot_l - l.x, 2 * normal.y * n_dot_l - l.y, 2 * normal.z * n_dot_l - l.z };
                float r_dot_v = dotProduct(r, viewport);
                if (r_dot_v > 0)
                {
                    intensity += current_light->intensity * powf(r_dot_v/(lengthVector(r)*lengthVector(viewport)), specular);
                }
            }
        }
    }

    return intensity;
}

void closestIntersection(Sphere** closest_sphere, float* closest_t, Canvas* canvas, Point3D starting_point, Vec3 direction, float t_min, float t_max)
{
    float t1, t2 = 0;
    for (int i = 0; i < canvas->scene->sphere_count; i++)
    {
        Sphere *current_sphere = &canvas->scene->spheres[i];
        intersectRaySphere(&t1, &t2, starting_point, direction, *current_sphere);

        if (t1 >= t_min && t1 <= t_max && t1 < *closest_t)
        {
            *closest_t = t1;
            *closest_sphere = current_sphere;
        }
        if (t2 >= t_min && t2 <= t_max && t2 < *closest_t)
        {
            *closest_t = t2;
            *closest_sphere = current_sphere;
        }
    }
}

uint32_t traceRay(Canvas* canvas, Point3D starting_point, Vec3 direction, float t_min, float t_max)
{
    float closest_t = INFINITY;
    Sphere *closest_sphere = 0;

    closestIntersection(&closest_sphere, &closest_t, canvas, starting_point, direction, t_min, t_max);

    if (!closest_sphere)
    {
        return canvas->background_color;
    }

    Point3D point = { starting_point.x + closest_t * direction.x, starting_point.y + closest_t * direction.y, starting_point.z + closest_t * direction.z };
    Vec3 normal = (Vec3) { point.x - closest_sphere->center.x, point.y - closest_sphere->center.y, point.z - closest_sphere->center.z };

    normalizeVector(&normal);
    float intensity = computeLighting(canvas, point, normal, (Vec3) { -direction.x, -direction.y, -direction.z } , closest_sphere->specular);

    return colorMultiply(closest_sphere->color, intensity);

    // return COLOR_ARGB(ALPHA(closest_sphere->color), red, green, blue);
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

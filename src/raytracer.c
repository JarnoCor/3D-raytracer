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

void closestIntersection(Sphere** closest_sphere, float* closest_t, Canvas* canvas, Point3D starting_point, Vec3 direction, float t_min, float t_max)
{
    *closest_t = INFINITY;
    *closest_sphere = 0;

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

Vec3 reflectRay(Vec3 ray, Vec3 normal)
{
    float dot_product = dotProduct(normal, ray);
    return (Vec3) { 2 * normal.x * dot_product - ray.x, 2 * normal.y * dot_product - ray.y, 2 * normal.z * dot_product - ray.z };
}

float collectShadow(Canvas* canvas, Point3D point, Vec3 light_direction, float t_min, float t_max)
{
    float light_amount = 1.0f;
    float t1, t2;

    for (int i = 0; i < canvas->scene->sphere_count; i++)
    {
        Sphere *sphere = &canvas->scene->spheres[i];
        intersectRaySphere(&t1, &t2, point, light_direction, *sphere);

        if (t1 >= t_min && t1 < t_max)
        {
            light_amount *= sphere->transparancy * 1.25f;
            if (light_amount <= 0.0f)
            {
                return 1.0f;
            }
        }

        if (t2 >= t_min && t2 < t_max)
        {
            light_amount *= sphere->transparancy  * 1.25f;
            if (light_amount <= 0.0f)
            {
                return 1.0f;
            }
        }
    }

    return 1-light_amount;
}

float computeLighting(Canvas* canvas, Point3D point, Vec3 normal, Vec3 viewport, float specular, float transparancy)
{

    (void)transparancy;

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
            float t_max;
            if (current_light->type == LIGHT_POINT)
            {
                l = (Vec3) { current_light->data.point.position.x - point.x, current_light->data.point.position.y - point.y, current_light->data.point.position.z - point.z };
                t_max = current_light->data.point.range;
            } else
            {
                l = current_light->data.directional.direction;
                t_max = INFINITY;
            }

            float shadow = collectShadow(canvas, point, l, 0.001, t_max);
            if (shadow >= 1.0f)
            {
                continue;
            }

            // diffuse
            float n_dot_l = dotProduct(normal, l);
            if (n_dot_l > 0)
            {
                intensity += (1-shadow) * current_light->intensity * n_dot_l/(lengthVector(normal) * lengthVector(l));
            }

            // specular
            if (specular != -1)
            {
                Vec3 r = reflectRay(l, normal);
                float r_dot_v = dotProduct(r, viewport);
                if (r_dot_v > 0)
                {
                    intensity += (1-shadow) * current_light->intensity * powf(r_dot_v/(lengthVector(r)*lengthVector(viewport)), specular);
                }
            }
        }
    }

    return intensity;
}

uint32_t traceRay(Canvas* canvas, Point3D starting_point, Vec3 direction, float t_min, float t_max, int recursion_depth, Stack *refraction_indexes)
{
    float closest_t;
    Sphere *closest_sphere;

    closestIntersection(&closest_sphere, &closest_t, canvas, starting_point, direction, t_min, t_max);

    if (!closest_sphere)
    {
        return canvas->background_color;
    }

    // compute local color
    Point3D point = { starting_point.x + closest_t * direction.x, starting_point.y + closest_t * direction.y, starting_point.z + closest_t * direction.z };
    Vec3 normal = (Vec3) { point.x - closest_sphere->center.x, point.y - closest_sphere->center.y, point.z - closest_sphere->center.z };

    normalizeVector(&normal);
    float intensity = computeLighting(canvas, point, normal, (Vec3) { -direction.x, -direction.y, -direction.z } , closest_sphere->specular, closest_sphere->transparancy);

    uint32_t computed_color = colorMultiply(closest_sphere->color, intensity);

    // if we hit the recursion limit or the object is not reflective, we're done
    float reflection = closest_sphere->reflective;
    if (recursion_depth > 0 && reflection > 0)
    {
        // compute the reflected color
        Vec3 ray = reflectRay((Vec3) { -direction.x, -direction.y, -direction.z }, normal);
        uint32_t reflected_color = traceRay(canvas, point, ray, 0.001, INFINITY, recursion_depth - 1, refraction_indexes);

        computed_color = colorAdd(colorMultiply(computed_color, 1-reflection), colorMultiply(reflected_color, reflection));
    }

    float transparancy = closest_sphere->transparancy;
    if (transparancy > 0)
    {
        // calculate the new direction based on the current and new index of refractions
        float cos_i = dotProduct(direction, normal);
        float n1 = 1.0f;
        float n2 = 1.0f;

        Vec3 n = normal;

        if (cos_i <= 0) {
            n1 = getHead(refraction_indexes);
            n2 = closest_sphere->ior;
            push(refraction_indexes, closest_sphere->ior);
            cos_i = -cos_i;
        } else {
            n1 = closest_sphere->ior;
            n2 = getHead(refraction_indexes);
            pop(refraction_indexes);
            n = (Vec3) { -normal.x, -normal.y, -normal.z };
        }

        float eta = n1/n2;
        float k = 1.0f - eta*eta*(1.0f - cos_i*cos_i);

        if (k >= 0.0f)
        {
            Vec3 new_direction = (Vec3) {
                eta*direction.x + (eta*cos_i - sqrtf(k))*n.x,
                eta*direction.y + (eta*cos_i - sqrtf(k))*n.y,
                eta*direction.z + (eta*cos_i - sqrtf(k))*n.z
            };

            normalizeVector(&new_direction);

            uint32_t transparent_color = traceRay(canvas, point, new_direction, 0.001, INFINITY, recursion_depth, refraction_indexes);

            computed_color = colorAdd(colorMultiply(computed_color, 1-transparancy), colorMultiply(transparent_color, transparancy));
        }
    }

    return computed_color;
}

void render(Canvas* canvas)
{
    Point3D origin = { 0, 0, 0 };

    for (int x = -canvas->width/2; x < canvas->width/2; x++)
    {
        for (int y = canvas->height/2; y > -canvas->height/2; y--)
        {
            Point3D view_direction = canvasToViewport(canvas, canvas->scene->viewport, x, y);
            Stack refraction_indexes;
            initializeStack(&refraction_indexes);
            push(&refraction_indexes, 1.0f);
            uint32_t color = traceRay(canvas, origin, view_direction, 1, INFINITY, 3, &refraction_indexes);
            setPixel(canvas, x, y, color);
        }
    }
}

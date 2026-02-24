#ifndef LIGHT_H
#define LIGHT_H

#include "util.h"

typedef enum {
    LIGHT_POINT,
    LIGHT_DIRECTIONAL,
    LIGHT_AMBIENT
} LightType;

typedef struct Light
{
    LightType type;
    float intensity;

    union
    {
        struct
        {
            Point3D position;
        } point;

        struct
        {
            Vec3 direction;
        } directional;

        struct
        {
            // ambient has no extra data
        } ambient;

    } data;

} Light;


#endif
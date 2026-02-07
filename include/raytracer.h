#ifndef TRACER_H
#define TRACER_H

#include "windows/canvas.h"
#include <math.h>

typedef struct Canvas Canvas;

void render(Canvas* canvas);

uint32_t traceRay(Canvas* canvas, Point3D camera, Point3D viewport, float t_min, float t_max);

#endif
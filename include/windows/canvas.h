#ifndef CANVAS_H
#define CANVAS_H

#include "basewin.h"
#include "scene.h"
#include "raytracer.h"
#include <stdint.h>

typedef struct Canvas {
    BaseWindow base;
    int width;
    int height;
    uint32_t* pixels;
    BITMAPINFO bmi;
    Scene* scene;
    uint32_t background_color;
} Canvas;

// Initializes the canvas data structure
Canvas* createCanvas(int width, int height);

void destroyCanvas(Canvas* canvas);

void setPixel(Canvas *canvas, int x, int y, uint32_t color);

void drawCanvas(Canvas *canvas);

#endif

#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include <math.h>
#include <stdint.h>
// #include <Unknwn.h>
// #include <winuser.h>

#define COLOR_ARGB(a, r, g, b) (((a)&0xFF) << 24 | ((r)&0xFF) << 16 | ((g)&0xFF) << 8 | ((b)&0xFF))
#define ALPHA(color) (((color) >> 24)&0xFF)
#define RED(color) (((color) >> 16)&0xFF)
#define GREEN(color) (((color) >> 8)&0xFF)
#define BLUE(color) ((color)&0xFF)

#define STACK_SIZE 100

uint32_t colorMultiply(uint32_t, float);

uint32_t colorAdd(uint32_t, uint32_t);

void SafeRelease(IUnknown**);

extern float g_DPIScale;

// Initializes the DPI scale for this window
void InitializeDPIScale(HWND);

// Convert a physical pixel to a device-independent pixel
float PixelsToDipsX(float);

// Convert a physical pixel to a device-independent pixel
float PixelsToDipsY(float);

typedef struct Point2D {
    float x;
    float y;
} Point2D;

typedef struct Point3D {
    float x;
    float y;
    float z;
} Point3D;

typedef Point3D Vec3;

float dotProduct(Vec3, Vec3);

float lengthVector(Vec3);

void normalizeVector(Vec3*);

typedef struct Stack {
    int head;
    float values[STACK_SIZE];
} Stack;

void push(Stack*, float);

float pop(Stack*);

float getHead(Stack*);

void initializeStack(Stack*);

#endif

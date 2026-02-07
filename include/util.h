#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
// #include <Unknwn.h>
// #include <winuser.h>

#define COLOR_ARGB(a, r, g, b) (((a)&0xFF) << 24 | ((r)&0xFF) << 16 | ((g)&0xFF) << 8 | ((b)&0xFF))

void SafeRelease(IUnknown**);

extern float g_DPIScale;

// Initializes the DPI scale for this window
void InitializeDPIScale(HWND hwnd);

// Convert a physical pixel to a device-independent pixel
float PixelsToDipsX(float x);

// Convert a physical pixel to a device-independent pixel
float PixelsToDipsY(float y);

typedef struct Point2D {
    float x;
    float y;
} Point2D;

typedef struct Point3D {
    float x;
    float y;
    float z;
} Point3D;

float dotProduct(Point3D a, Point3D b);

#endif

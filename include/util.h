#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
// #include <Unknwn.h>
// #include <winuser.h>

void SafeRelease(IUnknown**);

extern float g_DPIScale;

// Initializes the DPI scale for this window
void InitializeDPIScale(HWND hwnd);

// Convert a physical pixel to a device-independent pixel
float PixelsToDipsX(float x);

// Convert a physical pixel to a device-independent pixel
float PixelsToDipsY(float y);

#endif

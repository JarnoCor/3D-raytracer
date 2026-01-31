#include "util.h"

void SafeRelease(IUnknown **pointer)
{
    if (*pointer)
    {
        (*pointer)->lpVtbl->Release(*pointer);
        *pointer = 0;
    }
}

float g_DPIScale = 1.0f;

void InitializeDPIScale(HWND hwnd)
{
    float dpi = GetDpiForWindow(hwnd);
    g_DPIScale = dpi / USER_DEFAULT_SCREEN_DPI;
}

float PixelsToDipsX(float x)
{
    return (float) (x / g_DPIScale);
}

float PixelsToDipsY(float y)
{
    return (float) (y / g_DPIScale);
}

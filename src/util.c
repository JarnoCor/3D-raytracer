#include "util.h"

uint32_t colorMultiply(uint32_t color, float intensity)
{
    uint8_t alpha = ALPHA(color);
    long long red_long = (long long)RED(color) * intensity;
    long long green_long = (long long)GREEN(color) * intensity;
    long long blue_long = (long long)BLUE(color) * intensity;

    uint8_t red = min(red_long, 255);
    uint8_t green = min(green_long, 255);
    uint8_t blue = min(blue_long, 255);


    return COLOR_ARGB(alpha, red, green, blue);
}

uint32_t colorAdd(uint32_t color1, uint32_t color2)
{
    int alpha_int = ALPHA(color1) + ALPHA(color2);
    int red_int = RED(color1) + RED(color2);
    int green_int = GREEN(color1) + GREEN(color2);
    int blue_int = BLUE(color1) + BLUE(color2);

    uint8_t alpha = min(alpha_int, 255);
    uint8_t red = min(red_int, 255);
    uint8_t green = min(green_int, 255);
    uint8_t blue = min(blue_int, 255);

    return COLOR_ARGB(alpha, red, green, blue);
}

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

float dotProduct(Vec3 a, Vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float lengthVector(Vec3 vector)
{
    return sqrtf(dotProduct(vector, vector));
}

void normalizeVector(Vec3 *vector)
{
    float length = lengthVector(*vector);

    vector->x = vector->x / length;
    vector->y = vector->y / length;
    vector->z = vector->z / length;
}

void push(Stack *stack, float value)
{
    if (stack->head < STACK_SIZE)
    {
        stack->values[stack->head] = value;
        stack->head++;
    }
}

float pop(Stack *stack)
{
    return stack->values[--stack->head];
}

float getHead(Stack *stack)
{
    return stack->values[stack->head-1];
}

void initializeStack(Stack *stack)
{
    stack->head = 0;
}

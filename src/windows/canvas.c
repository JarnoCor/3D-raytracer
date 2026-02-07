#include "windows/canvas.h"

void resizeCanvas(Canvas *canvas, int width, int height)
{
    canvas->width = width;
    canvas->height = height;

    if (canvas->pixels)
    {
        free(canvas->pixels);
    }

    canvas->pixels = (uint32_t*) malloc(width * height * sizeof(uint32_t));

    canvas->bmi.bmiHeader.biWidth = width;
    canvas->bmi.bmiHeader.biHeight = -height; // negative for top-down bitmap (origin in top-left corner)
}

void destroyCanvas(Canvas *canvas)
{
    if (canvas->pixels)
    {
        free(canvas->pixels);
    }
    free(canvas);
}

void setPixel(Canvas *canvas, int x, int y, uint32_t color)
{
    int screenX = x + canvas->width/2;
    int screenY = -y + canvas->height/2;

    if (screenX >= 0 && screenX < canvas->width && screenY >= 0 && screenY < canvas->height)
    {
        canvas->pixels[screenY * canvas->width + screenX] = color;
    }
}

void drawCanvas(Canvas *canvas)
{
    HDC hdc = GetDC(canvas->base.m_hwnd);
    StretchDIBits(
        hdc,
        0, 0, canvas->width, canvas->height,
        0, 0, canvas->width, canvas->height,
        canvas->pixels,
        &canvas->bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );

    ReleaseDC(canvas->base.m_hwnd, hdc);
}

void updateCanvas(Canvas *canvas)
{
    render(canvas);

    // for (int x = -canvas->width/2; x < canvas->width/2; x++)
    // {
    //     for (int y = canvas->height/2; y > -canvas->height/2; y--)
    //     {
    //         setPixel(canvas, x, y, COLOR_ARGB(255, 255, 255, 255));
    //     }
    // }

    drawCanvas(canvas);
}

LPCWSTR Canvas_ClassName()
{
    static LPCWSTR classname = L"Canvas";
    return classname;
}

static LRESULT Canvas_HandleMessage(
    BaseWindow *base,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    Canvas *canvas = (Canvas*) base;
    switch (uMsg)
    {
        case WM_CREATE:
            RECT rc;
            GetClientRect(base->m_hwnd, &rc);

            resizeCanvas(canvas, rc.right - rc.left, rc.bottom - rc.top);

            InitializeDPIScale(base->m_hwnd);
            return 0;

        case WM_DESTROY:
            destroyCanvas(canvas);
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            updateCanvas(canvas);
            return 0;

        case WM_SIZE:
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

            resizeCanvas(canvas, width, height);

            return 0;

        default:
            return DefWindowProc(base->m_hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}

static BaseWindowVTable canvas_vtable = {
    Canvas_ClassName,
    Canvas_HandleMessage
};

Canvas* createCanvas(int width, int height)
{
    Canvas *canvas = (Canvas*) malloc(sizeof(Canvas));

    BaseWindow base = {
        NULL,
        &canvas_vtable
    };

    canvas->base = base;
    canvas->width = width;
    canvas->height = height;
    canvas->pixels = 0; //(uint32_t*) malloc(width * height * sizeof(uint32_t));

    ZeroMemory(&canvas->bmi, sizeof(BITMAPINFO));
    canvas->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    canvas->bmi.bmiHeader.biWidth = width;
    canvas->bmi.bmiHeader.biHeight = -height; // negative for top-down bitmap (origin in top-left corner)
    canvas->bmi.bmiHeader.biPlanes = 1;
    canvas->bmi.bmiHeader.biBitCount = 32; // RGB
    canvas->bmi.bmiHeader.biCompression = BI_RGB;

    Sphere spheres[3];

    uint32_t red = COLOR_ARGB(255, 255, 0, 0);
    uint32_t green = COLOR_ARGB(255, 0, 255, 0);
    uint32_t blue = COLOR_ARGB(255, 0, 0, 255);
    spheres[0] = (Sphere) { 1, (Point3D) { 0, -1, 3 }, red };
    spheres[1] = (Sphere) { 1, (Point3D) { 2, 0, 4 }, blue };
    spheres[2] = (Sphere) { 1, (Point3D) { -2, 0, 4 }, green };

    canvas->scene = initializeScene(3, spheres);

    canvas->background_color = 0xFFFFFFFF; //(((uint8_t)255) << 24) | (((uint8_t)255) << 16) | (((uint8_t)255) << 8) | ((uint8_t)255);

    return canvas;
}

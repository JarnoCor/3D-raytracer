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

    float canvasAspect = (float) canvas->width / canvas->height;

    canvas->scene->viewport.width = canvasAspect;
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

    Sphere spheres[4];

    // uint32_t red = COLOR_ARGB(255, 255, 0, 0);
    uint32_t green = COLOR_ARGB(255, 100, 231, 100);
    uint32_t blue = COLOR_ARGB(255, 50, 50, 255);
    uint32_t white = COLOR_ARGB(255, 255, 255, 255);
    spheres[0] = (Sphere) { 1.1, (Point3D) { 0, -0.7, 4.5 }, white, 1000, 1.0f, 0.9f, 1.007f };
    spheres[1] = (Sphere) { 1, (Point3D) { 1.25, -1, 6.3 }, blue, 500, 0.2f, 0, 1 };
    spheres[2] = (Sphere) { 1, (Point3D) { -1.9, -1, 5.5 }, green, 10, 0.1f, 0, 1 };
    spheres[3] = (Sphere) { 5000, (Point3D) { 0, -5002, 0 }, COLOR_ARGB(255, 255, 128, 0), 1000, 0.0f, 0, 1 };

    Light lights[3];

    Light ambient = {
        .type = LIGHT_AMBIENT,
        .intensity = 0.2f
    };

    Light point = {
        .type = LIGHT_POINT,
        .intensity = 0.6f,
        .data.point.position = (Point3D) { 2, 1, 0 },
        .data.point.range = 0.5
    };

    Light directional = {
        .type = LIGHT_DIRECTIONAL,
        .intensity = 0.2f,
        .data.directional.direction = (Vec3) { 1, 4, 4 }
    };

    lights[0] = ambient;
    lights[1] = point;
    lights[2] = directional;

    canvas->scene = initializeScene(spheres, 4, lights, 3);

    canvas->background_color = COLOR_ARGB(255, 255, 255, 255);

    return canvas;
}

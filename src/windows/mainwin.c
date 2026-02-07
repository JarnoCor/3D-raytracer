#include "windows/mainwin.h"
#include <stdio.h>

LPCWSTR MainWindow_ClassName()
{
    static LPCWSTR classname = L"Main Window Class";
    return classname;
}

static LRESULT MainWindow_HandleMessage(
    BaseWindow *selfBase,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    MainWindow *self = (MainWindow*)selfBase;
    switch (uMsg)
    {
    case WM_CREATE:
    if(FAILED(D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &IID_ID2D1Factory, NULL, (void**)&self->factory)))
        {
            return -1; // Fail CreateWindowEx
        }
        InitializeDPIScale(selfBase->m_hwnd);
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResources(self);
        SafeRelease((IUnknown**)&self->factory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaint(self);
        return 0;

    case WM_SIZE:
        Resize(self);
        return 0;

    default:
        return DefWindowProc(selfBase->m_hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}

static BaseWindowVTable mainwindow_vtable = {
    MainWindow_ClassName,
    MainWindow_HandleMessage
};

void MainWindow_Init(MainWindow *self)
{
    self->base.m_hwnd = NULL;
    self->base.vtable = &mainwindow_vtable;

    self->factory = NULL;
    self->renderTarget = NULL;
    self->brush = NULL;
}

void debug_printf(const char* fmt, ...) {
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // Convert to wide string
    wchar_t wbuffer[512];
    MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wbuffer, 512);

    OutputDebugString(wbuffer);
}

void CalculateLayout(MainWindow *self)
{
    if (self->renderTarget != NULL)
    {
        RECT rc;
        GetClientRect(((BaseWindow*)self)->m_hwnd, &rc);
        D2D1_SIZE_F size = (D2D1_SIZE_F) {rc.right, rc.bottom};

        const float x = PixelsToDipsX(size.width) / 2;
        const float y = PixelsToDipsY(size.height) / 2;
        const float radius = min(x, y);
        self->ellipse = (D2D1_ELLIPSE) {(D2D1_POINT_2F) {x, y}, radius, radius};
    }
}

HRESULT CreateGraphicsResources(MainWindow *self)
{
    HRESULT hr = S_OK;

    if (self->factory == NULL)
    {
        hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &IID_ID2D1Factory,
            NULL,
            (void**)&self->factory
        );
        if (FAILED(hr))
        {
            return hr;
        }
    }

    if (self->renderTarget == NULL)
    {
        RECT rc;
        GetClientRect(self->base.m_hwnd, &rc);

        D2D1_SIZE_U size = (D2D1_SIZE_U) {rc.right, rc.bottom};

        hr = self->factory->lpVtbl->CreateHwndRenderTarget(
            self->factory,
            &(D2D1_RENDER_TARGET_PROPERTIES) {D2D1_RENDER_TARGET_TYPE_DEFAULT, (D2D1_PIXEL_FORMAT) {DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_UNKNOWN}, 0.0f, 0.0f, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT},
            &(D2D1_HWND_RENDER_TARGET_PROPERTIES) {self->base.m_hwnd, size, D2D1_PRESENT_OPTIONS_NONE},
            &self->renderTarget
        );

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = (D2D1_COLOR_F) {1.0f, 1.0f, 0.0f, 1.0f};
            const D2D1_MATRIX_3X2_F identityMatrix = (D2D1_MATRIX_3X2_F) {{{1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}}};
            const D2D1_BRUSH_PROPERTIES properties = (D2D1_BRUSH_PROPERTIES) {1.0f, identityMatrix};
            hr = self->renderTarget->lpVtbl->Base.CreateSolidColorBrush((ID2D1RenderTarget*)self->renderTarget, &color, &properties, &self->brush);

            if (SUCCEEDED(hr))
            {
                CalculateLayout(self);
            }
        }
    }

    return hr;
}

void DiscardGraphicsResources(MainWindow *self)
{
    SafeRelease((IUnknown**)&self->renderTarget);
    SafeRelease((IUnknown**)&self->brush);
}

void OnPaint(MainWindow *self)
{
    HRESULT hr = CreateGraphicsResources(self);

    if (SUCCEEDED(hr) && self->renderTarget != NULL)
    {
        PAINTSTRUCT ps;
        BeginPaint(((BaseWindow*)self)->m_hwnd, &ps);

        ID2D1RenderTarget *rt = (ID2D1RenderTarget*)self->renderTarget;

        rt->lpVtbl->BeginDraw(rt);

        const D2D1_COLOR_F skyBlue = (D2D1_COLOR_F) {135.0f/255.0f, 206.0f/255.0f, 235.0f/255.0f, 1.0f};
        rt->lpVtbl->Clear(rt, &skyBlue);
        rt->lpVtbl->FillEllipse(rt, &self->ellipse, (ID2D1Brush*)self->brush);

        D2D1_TAG tag1, tag2;
        hr = rt->lpVtbl->EndDraw(rt, &tag1, &tag2);
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources(self);
        }

        EndPaint(((BaseWindow*)self)->m_hwnd, &ps);
    }
}

void Resize(MainWindow *self)
{
    if (self->renderTarget != NULL)
    {
        RECT rc;
        GetClientRect(((BaseWindow*)self)->m_hwnd, &rc);

        D2D1_SIZE_U size = (D2D1_SIZE_U) {rc.right, rc.bottom};

        self->renderTarget->lpVtbl->Resize(self->renderTarget, &size);
        CalculateLayout(self);
        InvalidateRect(((BaseWindow*)self)->m_hwnd, NULL, FALSE);
    }
}

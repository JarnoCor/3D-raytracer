#include "windows/basewin.h"

BOOL BaseWindow_Create(
    BaseWindow *self,
    LPWSTR lpWindowName,
    DWORD dwStyle,
    DWORD dwExStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu
)
{
    WNDCLASS wc = {0};

    wc.lpfnWndProc = BaseWindow_WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = self->vtable->ClassName();

    RegisterClass(&wc);

    self->m_hwnd = CreateWindowEx(
        dwExStyle,
        self->vtable->ClassName(),
        lpWindowName,
        dwStyle,
        x,
        y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        GetModuleHandle(NULL),
        self
    );

    return self->m_hwnd != NULL;
}

BOOL BaseWindow_CreateDefault(
    BaseWindow *self,
    LPWSTR lpWindowName,
    DWORD dwStyle,
    int width,
    int height
)
{
    return BaseWindow_Create(
        self,
        lpWindowName,
        dwStyle,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        0,
        0
    );
}

LRESULT CALLBACK BaseWindow_WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    BaseWindow *self = NULL;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT *create = (CREATESTRUCT*) lParam;
        self = (BaseWindow*) create->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) self);

        self->m_hwnd = hwnd;
    } else {
        self = (BaseWindow*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (self) {
        return self->vtable->HandleMessage(self, uMsg, wParam, lParam);
    } else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND Window(BaseWindow* self)
{
    return self->m_hwnd;
}

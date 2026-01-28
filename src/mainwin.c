#include "mainwin.h"

LPCWSTR MainWindow_ClassName()
{
    static LPCWSTR classname = L"Main Window Class";
    return classname;
}

static LRESULT MainWindow_HandleMessage(
    BaseWindow *self,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(self->m_hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(self->m_hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(self->m_hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}

static BaseWindowVTable mainwindows_vtable = {
    MainWindow_ClassName,
    MainWindow_HandleMessage
};

void MainWindow_Init(MainWindow *self)
{
    self->base.m_hwnd = NULL;
    self->base.vtable = &mainwindows_vtable;
}

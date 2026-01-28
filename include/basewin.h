#ifndef BASEWIN_H
#define BASEWIN_H

#include <windows.h>

typedef struct BaseWindow BaseWindow;

// the table for virtual functions
typedef struct {
    LPCWSTR (*ClassName)(void);
    LRESULT (*HandleMessage)(
        BaseWindow *self,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
    );
} BaseWindowVTable;

// Base window struct
struct BaseWindow {
    // Handle to the application instance
    HWND m_hwnd;
    // Table with window virtual functions
    const BaseWindowVTable *vtable;
};

// Creates a base window
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
);

// Creates a base window with default parameters
BOOL BaseWindow_CreateDefault(
    BaseWindow *self,
    LPWSTR lpWindowName,
    DWORD dwStyle
);

// Base window procedure
LRESULT CALLBACK BaseWindow_WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

// Get the handle to the window instance
HWND Window(BaseWindow* self);

#endif

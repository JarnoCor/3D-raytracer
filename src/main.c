#include "mainwin.h"
#include <stdio.h>

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)pCmdLine;

    MainWindow win;
    MainWindow_Init(&win);

    if(!BaseWindow_CreateDefault(&win.base, L"Circle", WS_OVERLAPPEDWINDOW))
    {
        DWORD err = GetLastError();
        printf("CreateDefault failed, GetLastError=%lu\n", err);
        return 1;
    }

    ShowWindow(Window(&win.base), nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("Message loop exited, msg.message=%u msg.wParam=%lu\n", (unsigned)msg.message, (unsigned long)msg.wParam);

    return 0;
}

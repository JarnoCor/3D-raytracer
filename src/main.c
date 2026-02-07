#include "windows/canvas.h"
#include <stdio.h>

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)pCmdLine;

    Canvas *canvas = createCanvas(500, 500);

    if(!BaseWindow_CreateDefault(&canvas->base, L"Raytracing", WS_OVERLAPPEDWINDOW, 500, 500))
    {
        DWORD err = GetLastError();
        printf("CreateDefault failed, GetLastError=%lu\n", err);
        return 1;
    }

    ShowWindow(Window(&canvas->base), nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("Message loop exited, msg.message=%u msg.wParam=%lu\n", (unsigned)msg.message, (unsigned long)msg.wParam);

    return 0;
}

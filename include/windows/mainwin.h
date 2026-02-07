#ifndef MAIN_WIN_H
#define MAIN_WIN_H

#include "basewin.h"
#include <d2d1.h>
// #pragma comment(lib, "d2d1")

typedef struct {
    BaseWindow base;

    ID2D1Factory *factory;
    ID2D1HwndRenderTarget *renderTarget;
    ID2D1SolidColorBrush *brush;
    D2D1_ELLIPSE ellipse;
} MainWindow;

// Initializes the main window data structure
void MainWindow_Init(MainWindow *self);

// Recalculate drawing layout when the size of the window changes
void CalculateLayout(MainWindow *self);

HRESULT CreateGraphicsResources(MainWindow *self);

void DiscardGraphicsResources(MainWindow *self);

void OnPaint(MainWindow *self);

void Resize(MainWindow *self);

#endif

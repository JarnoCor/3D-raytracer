#ifndef Main_WINDOW_H
#define Main_WINDOW_H

#include "basewin.h"

typedef struct {
    BaseWindow base;
} MainWindow;

// Initializes the main window data structure
void MainWindow_Init(MainWindow *self);

#endif

#include "util.h"

void SafeRelease(IUnknown **pointer)
{
    if (*pointer)
    {
        (*pointer)->lpVtbl->Release(*pointer);
        *pointer = 0;
    }
}
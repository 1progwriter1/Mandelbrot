#ifndef MANDELBROT_DATA
#define MANDELBROT_DATA

#include <stdio.h>

struct ImageData {
    size_t offset_x;
    size_t offset_y;
    float scale;
    bool is_window_closed;
};

#endif
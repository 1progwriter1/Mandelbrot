#ifndef MANDELBROT_DATA
#define MANDELBROT_DATA

#define MEASURE

#define NO_AVX_FILE     "time_no_avx.txt"
#define NO_AVX_8_FILE   "time_no_avx_8.txt"
#define AVX_FILE        "time_avx.txt"

#include <stdio.h>
#include <SFML/Graphics.hpp>

const size_t NUMBER_OF_MEASUREMENTS = 1000;
const size_t NUMBER_OF_SCREENS      = 100;

const unsigned int SCREEN_WIDTH  = 800;
const unsigned int SCREEN_HEIGHT = 600;

const float STEP_X = 1/200.f;
const float STEP_Y = 1/150.f;

const size_t MAX_DOT_INDEX = 256;
const float MAX_RADIUS_SQUARE = 100.f;

struct WindowData {

    unsigned int width;
    unsigned int height;

    float dx;
    float dy;

    float offset_x;
    float offset_y;
    float scale;
    float scale_ratio;
};

#endif


/**
 * TODO: all tips below
 * * условная компиляция
 * * учесть рисование при расчете времени
 * * godbolt
 * * why simd with -O3
 * * погрешность
 * * измерять работу всей проги
 * * zoom using shift
 * ! -O2 for no_avx_8.cpp instead of -O3
 */
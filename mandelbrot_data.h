#ifndef MANDELBROT_DATA
#define MANDELBROT_DATA

#include <stdio.h>
#include <SFML/Graphics.hpp>

const unsigned int SCREEN_WIDTH  = 800;
const unsigned int SCREEN_HEIGHT = 600;

const float STEP_X = 1/200.f;
const float STEP_Y = 1/150.f;

const size_t MAX_DOT_INDEX = 100;
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
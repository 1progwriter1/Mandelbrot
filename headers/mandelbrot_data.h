#ifndef MANDELBROT_DATA
#define MANDELBROT_DATA

#define AVX_TIME        "Results/avx_time.txt"
#define ARRAYS_TIME     "Results/arrays_time.txt"
#define SIMPLE_TIME     "Results/simple_time.txt"
#define SIMPLE_TICKS    "Results/simple_ticks.txt"
#define ARRAYS_TICKS    "Results/arrays_ticks.txt"
#define AVX_TICKS       "Results/avx_ticks.txt"

#define FONT_FILE       "Arial Black.ttf"

#include <stdio.h>
#include <SFML/Graphics.hpp>

enum ExitCodes {
    SUCCESS = 0,
    ERROR = 1,
    FILE_OPEN_ERROR = 2,
};

const float DEFAULT_ZOOM = 12/10.f;
const float BIG_ZOOM     = 20/10.f;

const size_t NUMBER_OF_MEASUREMENTS = 10;
const size_t NUMBER_OF_SCREENS      = 100;

const unsigned int SCREEN_WIDTH  = 800;
const unsigned int SCREEN_HEIGHT = 600;

const float STEP_X = 1/200.f;
const float STEP_Y = 1/150.f;

const size_t MAX_DOT_INDEX = 256;
const float MAX_RADIUS_SQUARE = 100.f;

const size_t FPS_BUFFER_LEN = 64;

struct FpsTextData {

    sf::Text text;
    sf::Font font;
    sf::Clock clock;
};

struct WindowData {

    float dx;
    float dy;

    float offset_x;
    float offset_y;
    float scale;
    float scale_ratio;

    FpsTextData fps_data;
};

#endif


/**
 * * условная компиляция
 * * учесть рисование при расчете времени
 * TODO godbolt
 * TODO why simd with -O3
 * * погрешность
 * * измерять работу всей проги
 * * zoom using shift
 * * zoom from center
 * TODO incorrect image created in 2nd edition
 */
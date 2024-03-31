#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "mandelbrot_data.h"
#include "draw_func.h"
#include <assert.h>
#include <immintrin.h>

static void SetPixels(sf::VertexArray &pixels, WindowData *data);
static void CalculateDots(size_t *dots_indexes, float x0, float y0, float dx, float dy);

int main(const int argc, const char *argv[]) {

    assert(argv);

    WindowData data = {};
    SetWindowData(&data);
    sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);

    #ifndef MEASURE
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot");

    while (window.isOpen()) {

        ProceedKeyStrokes(window, &data);

        SetPixels(pixels, &data);

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.display();
    }
    #else
    FILE *time  = fopen(ARRAYS_TIME, "w");
    if (!time)  return FILE_OPEN_ERROR;
    FILE *ticks = fopen(ARRAYS_TICKS, "w");
    if (!ticks) {fclose(time); return FILE_OPEN_ERROR}

    for (size_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++) {

        unsigned long long start = __rdtsc();

        for (size_t j = 0; j < NUMBER_OF_SCREENS; j++)
            SetPixels(pixels, &data);

        unsigned long long end = __rdtsc();
        fprintf(fn, "%llu\n", end - start);
    }
    fclose(fn);
    #endif

    return SUCCESS;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data) {

    assert(data);

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    for (unsigned int y_index = 0; y_index < SCREEN_HEIGHT; y_index++) {

        float x0 = (-((float) SCREEN_WIDTH) / 2) * dx + data->offset_x * data->scale;
        float y0 = (((float) y_index) - (float) SCREEN_HEIGHT / 2) * dy + data->offset_y * data->scale * data->scale_ratio;

        for (unsigned int x_index = 0; x_index < SCREEN_WIDTH; x_index += 8, x0 += dx * 8) {

            size_t dots_indexes[8] = {};
            CalculateDots(dots_indexes, x0, y0, dx, dy);

            #ifndef MEASURE
            for (unsigned int i = 0; i < 8; i++) {
                size_t index = y_index * 800 + x_index + i;
                pixels[index].position = sf::Vector2f((float) (x_index + i),(float) y_index);
                if (dots_indexes[i] >= MAX_DOT_INDEX)   pixels[index].color = sf::Color::Black;
                else                                    pixels[index].color = sf::Color(255 - (char) dots_indexes[i],
                                                                                       (char) dots_indexes[i] % 8 * 32,
                                                                                       (char) dots_indexes[i]);;
            }
            #endif
        }
    }
}

static void CalculateDots(size_t *dots_indexes, float x0, float y0, float dx, float dy) {

    assert(dots_indexes);

    float X0[8] = {x0, x0 + dx, x0 + dx * 2, x0 + dx * 3, x0 + dx * 4, x0 + dx * 5, x0 + dx * 6, x0 + dx * 7};
    float Y0[8] = {y0, y0 + dy, y0 + dy * 2, y0 + dy * 3, y0 + dy * 4, y0 + dy * 5, y0 + dy * 6, y0 + dy * 7};

    float X[8] = {};    for (size_t i = 0; i < 8; i++) X[i] = X0[i];
    float Y[8] = {};    for (size_t i = 0; i < 8; i++) Y[i] = Y0[i];

    int mask_is_inside = 0;
    for (size_t i = 0; i < MAX_DOT_INDEX; i++) {

        float x2[8] = {};   for (size_t j = 0; j < 8; j++) x2[j] =  X[j] * X[j];
        float y2[8] = {};   for (size_t j = 0; j < 8; j++) y2[j] =  Y[j] * Y[j];
        float xy[8] = {};   for (size_t j = 0; j < 8; j++) xy[j] =  X[j] * Y[j];
        float r2[8] = {};   for (size_t j = 0; j < 8; j++) r2[j] = x2[j] + y2[j];

        mask_is_inside = 0;
        for (size_t j = 0; j < 8; j++)  mask_is_inside |= (r2[j] < MAX_RADIUS_SQUARE) << j;
        if (!mask_is_inside)  break;

        for (size_t j = 0; j < 8; j++)  dots_indexes[j] += (size_t) (mask_is_inside >> j) & 1;

        for (size_t j = 0; j < 8; j++)  X[j] = x2[j] - y2[j] + X0[j];
        for (size_t j = 0; j < 8; j++)  Y[j] = xy[j] + xy[j] + Y0[j];
    }
}

int MeasureProgram(sf::VertexArray &pixels, WindowData *data) {

    assert(data);

    FILE *time  = fopen(ARRAYS_TIME, "w");
    if (!time)  return FILE_OPEN_ERROR;

    FILE *ticks = fopen(ARRAYS_TICKS, "w");
    if (!ticks) {
        fclose(time);
        return FILE_OPEN_ERROR;
    }

    for (size_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++) {

        time_t time_start = clock();
        unsigned long long start = __rdtsc();

        for (size_t j = 0; j < NUMBER_OF_SCREENS; j++)
            SetPixels(pixels, data);

        unsigned long long end = __rdtsc();
        time_t time_end = clock();

        fprintf(ticks, "%llu\n", end - start);
        fprintf(time, "f\n", (double) (end - start) / CLOCKS_PER_SEC);
    }

    fclose(ticks);
    fclose(time);

}
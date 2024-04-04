#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "../headers/mandelbrot_data.h"
#include "../headers/draw_func.h"
#include <assert.h>
#include <immintrin.h>

static void SetPixels(sf::VertexArray &pixels, WindowData *data, bool if_measure);
static void CalculateDots(size_t *dots_indexes, float x0, float y0, float dx);
static int MeasureProgram(WindowData *data, const char *time_file, const char *ticks_file, float *test_sum);

int main(const int argc, const char *argv[]) {

    assert(argv);

    WindowData data = {};
    SetWindowData(&data);

    if (!IfMeasure(argc, argv)) {

        sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);
        sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot");

        while (window.isOpen()) {

            ProceedKeyStrokes(window, &data);

            SetPixels(pixels, &data, false);

            DrawWindow(window, pixels, &data);
        }
    }
    else {
        float test_sum = 0;
        if (MeasureProgram(&data, ARRAYS_TIME, ARRAYS_TICKS, &test_sum) != SUCCESS)
            return ERROR;
        printf("test sum: %f\n", test_sum);
    }

    return SUCCESS;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data, bool if_measure) {

    assert(data);

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    for (unsigned int y_index = 0; y_index < SCREEN_HEIGHT; y_index++) {

        float x0 = (-((float) SCREEN_WIDTH) / 2) * dx + data->offset_x;
        float y0 = (((float) y_index) - (float) SCREEN_HEIGHT / 2) * dy + data->offset_y;

        for (unsigned int x_index = 0; x_index < SCREEN_WIDTH; x_index += 8, x0 += dx * 8) {

            size_t dots_indexes[8] = {};
            CalculateDots(dots_indexes, x0, y0, dx);

           if (if_measure) {
                pixels[y_index * SCREEN_WIDTH + x_index].position.x = (float) dots_indexes[rand() % 8];
                continue;
            }

            for (unsigned int i = 0; i < 8; i++) {
                size_t index = y_index * 800 + x_index + i;
                pixels[index].position = sf::Vector2f((float) (x_index + i),(float) y_index);
                if (dots_indexes[i] >= MAX_DOT_INDEX)   pixels[index].color = sf::Color::Black;
                else                                    pixels[index].color = sf::Color(255 - (char) dots_indexes[i],
                                                                                       (char) dots_indexes[i] % 8 * 32,
                                                                                       (char) dots_indexes[i]);;
            }
        }
    }
}

static void CalculateDots(size_t *dots_indexes, float x0, float y0, float dx) {

    assert(dots_indexes);

    float X0[8] = {x0, x0 + dx, x0 + dx * 2, x0 + dx * 3, x0 + dx * 4, x0 + dx * 5, x0 + dx * 6, x0 + dx * 7};

    float X[8] = {};    for (size_t i = 0; i < 8; i++) X[i] = X0[i];
    float Y[8] = {};    for (size_t i = 0; i < 8; i++) Y[i] = y0;

    int mask_is_inside = 0;
    for (size_t i = 0; i < MAX_DOT_INDEX; i++) {

        float x2[8] = {};   for (size_t j = 0; j < 8; j++) x2[j] =  X[j] * X[j];
        float y2[8] = {};   for (size_t j = 0; j < 8; j++) y2[j] =  Y[j] * Y[j];
        float xy[8] = {};   for (size_t j = 0; j < 8; j++) xy[j] =  X[j] * Y[j];
        float r2[8] = {};   for (size_t j = 0; j < 8; j++) r2[j] = x2[j] + y2[j];

        unsigned int cmp[8] = {};
        unsigned int mask = 0;
        for (int j = 0; j < 8; j++) cmp[j] = (r2[j] <= MAX_RADIUS_SQUARE);
        for (int j = 0; j < 8; j++) mask |= cmp[j];

        if (!mask)  break;

        for (int j = 0; j < 8; j++) dots_indexes[j] += cmp[j];

        for (size_t j = 0; j < 8; j++)  X[j] = x2[j] - y2[j] + X0[j];
        for (size_t j = 0; j < 8; j++)  Y[j] = xy[j] + xy[j] + y0;
    }
}

static int MeasureProgram(WindowData *data, const char *time_file, const char *ticks_file, float *test_sum) {

    assert(data);
    assert(time_file);
    assert(ticks_file);
    assert(test_sum);

    FILE *time  = fopen(time_file, "w");
    if (!time)  return FILE_OPEN_ERROR;

    FILE *ticks = fopen(ticks_file, "w");
    if (!ticks) {
        fclose(time);
        return FILE_OPEN_ERROR;
    }

    float tmp_sum = 0;
    sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);
    for (size_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++) {

        printf("measurement №%lu\n", i + 1);

        time_t time_start = clock();
        unsigned long long start = __rdtsc();

        for (size_t j = 0; j < NUMBER_OF_SCREENS; j++) {
            SetPixels(pixels, data, true);
            tmp_sum += pixels[(unsigned int) rand() % SCREEN_HEIGHT * SCREEN_WIDTH].position.x;
        }

        unsigned long long end = __rdtsc();
        time_t time_end = clock();

        fprintf(ticks, "%llu\n", end - start);
        fprintf(time, "%f\n", (double) (time_end - time_start) / CLOCKS_PER_SEC);
    }

    fclose(ticks);
    fclose(time);

    *test_sum = tmp_sum;

    return SUCCESS;
}
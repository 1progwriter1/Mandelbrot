#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "../headers/mandelbrot_data.h"
#include "../headers/draw_func.h"
#include <assert.h>
#include <immintrin.h>

static void SetPixels(sf::VertexArray &pixels, WindowData *data, bool if_measure);
static size_t CalculateDot(float x0, float y0);
static int MeasureProgram(WindowData *data, const char *time_file, const char *ticks_file);

int main(const int argc, const char *argv[]) {

    WindowData data = {};
    SetWindowData(&data);

    if (!IfMeasure(argc, argv)) {

        sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);
        sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot");

        while (window.isOpen()) {

            ProceedKeyStrokes(window, &data);

            SetPixels(pixels, &data, false);

            window.clear(sf::Color::Black);
            window.draw(pixels);
            window.display();
        }
    }
    else {
        return MeasureProgram(&data, SIMPLE_TIME, SIMPLE_TICKS);
    }

    return SUCCESS;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data, bool if_measure) {

    assert(data);

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    for (unsigned int y_index = 0; y_index < SCREEN_WIDTH; y_index++) {

        float x0 = (-((float) SCREEN_WIDTH) / 2) * dx + data->offset_x * data->scale;
        float y0 = (((float) y_index) - (float) SCREEN_HEIGHT / 2) * dy + data->offset_y * data->scale * data->scale_ratio;

        for (unsigned int x_index = 0; x_index < SCREEN_WIDTH; x_index++, x0 += dx) {

            size_t dot_index = CalculateDot(x0, y0);

            if (if_measure) continue;

            size_t index = y_index * SCREEN_WIDTH + x_index;
            pixels[index].position = sf::Vector2f((float) x_index, (float) y_index);
            if (dot_index >= MAX_DOT_INDEX) pixels[index].color = sf::Color::Black;
            else                            pixels[index].color = sf::Color(255 - (char) dot_index,
                                                                           (char) dot_index % 8 * 32,
                                                                           (char) dot_index);
        }
    }

}

static size_t CalculateDot(float x0, float y0) {

    float x = x0;
    float y = y0;

    size_t dot_index = 0;
    for (; dot_index < MAX_DOT_INDEX; dot_index++) {

        float x2 = x * x;
        float y2 = y * y;
        float xy = x * y;
        float radius_square = x2 + y2;

        if (radius_square >= MAX_RADIUS_SQUARE) break;

        x = x2 - y2 + x0;
        y = xy + xy + y0;
    }

    return dot_index;
}

static int MeasureProgram(WindowData *data, const char *time_file, const char *ticks_file) {

    assert(data);
    assert(time_file);
    assert(ticks_file);

    FILE *time  = fopen(time_file, "w");
    if (!time)  return FILE_OPEN_ERROR;

    FILE *ticks = fopen(ticks_file, "w");
    if (!ticks) {
        fclose(time);
        return FILE_OPEN_ERROR;
    }

    sf::VertexArray pixels = {};

    for (size_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++) {

        time_t time_start = clock();
        unsigned long long start = __rdtsc();

        for (size_t j = 0; j < NUMBER_OF_SCREENS; j++)
            SetPixels(pixels, data, true);

        unsigned long long end = __rdtsc();
        time_t time_end = clock();

        fprintf(ticks, "%llu\n", end - start);
        fprintf(time, "%f\n", (double) (time_end - time_start) / CLOCKS_PER_SEC);
    }

    fclose(ticks);
    fclose(time);

    return SUCCESS;
}
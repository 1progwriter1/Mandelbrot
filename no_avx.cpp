#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "mandelbrot_data.h"
#include "draw_func.h"
#include <assert.h>
#include <immintrin.h>

static void SetPixels(sf::VertexArray &pixels, WindowData *data);
static size_t CalculateDot(float x0, float y0);

int main() {

    WindowData data = {};
    SetWindowData(&data);

    sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);

    #ifndef MEASURE
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot");
    while (window.isOpen()) {

        ProceedKeyStrokes(window, &data);
        if (!window.isOpen())   break;

        SetPixels(pixels, &data);

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.display();
    }
    #else
    FILE *fn = fopen(NO_AVX_FILE, "w");
    if (!fn)    return 1;

    for (size_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++) {

        unsigned long long start = __rdtsc();

        for (size_t j = 0; j < NUMBER_OF_SCREENS; j++)
            SetPixels(pixels, &data);

        unsigned long long end = __rdtsc();
        fprintf(fn, "%llu\n", end - start);
    }
    fclose(fn);
    #endif

    return 0;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data) {

    assert(data);

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    for (unsigned int y_index = 0; y_index < data->height; y_index++) {

        float x0 = (-((float) data->width) / 2) * dx + data->offset_x * data->scale;
        float y0 = (((float) y_index) - (float) data->height / 2) * dy + data->offset_y * data->scale * data->scale_ratio;

        for (unsigned int x_index = 0; x_index < data->width; x_index++, x0 += dx) {

            size_t dot_index = CalculateDot(x0, y0);

            #ifndef MEASURE
            size_t index = y_index * data->width + x_index;
            pixels[index].position = sf::Vector2f((float) x_index, (float) y_index);
            if (dot_index >= MAX_DOT_INDEX) pixels[index].color = sf::Color::Black;
            else                            pixels[index].color = sf::Color(255 - (char) dot_index,
                                                                           (char) dot_index % 8 * 32,
                                                                           (char) dot_index);
            #endif
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
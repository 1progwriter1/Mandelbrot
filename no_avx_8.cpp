#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "mandelbrot_data.h"
#include "draw_func.h"
#include <assert.h>
#include <immintrin.h>


static void SetPixels(sf::VertexArray &pixels, WindowData *data, FILE *fn);

int main() {

    WindowData data = {};
    SetWindowData(&data);

    FILE *fn = fopen(NO_AVX_8_FILE, "w");
    if (!fn)    return 1;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mandelbrot");
    sf::VertexArray pixels(sf::Points, 800 * 600);

    while (window.isOpen()) {

        ProceedKeyStrokes(window, &data);

        SetPixels(pixels, &data, fn);

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.display();
    }

    fclose(fn);

    return 0;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data, FILE *fn) {

    assert(data);

    #ifdef MEASURE
    assert(fn);
    unsigned long long start = __rdtsc();
    #endif

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    for (unsigned int y_index = 0; y_index < 600; y_index++) {

        float x_0 = (-((float) data->width) / 2) * dx + data->offset_x * data->scale;
        float y_0 = (((float) y_index) - (float) data->height / 2) * dy + data->offset_y * data->scale * data->scale_ratio;

        for (unsigned int x_index = 0; x_index < 800; x_index += 8, x_0 += dx * 8) {

            float X0[8] = {x_0, x_0 + dx, x_0 + dx * 2, x_0 + dx * 3, x_0 + dx * 4, x_0 + dx * 5, x_0 + dx * 6, x_0 + dx * 7};
            float Y0[8] = {y_0, y_0 + dy, y_0 + dy * 2, y_0 + dy * 3, y_0 + dy * 4, y_0 + dy * 5, y_0 + dy * 6, y_0 + dy * 7};

            float X[8] = {};    for (size_t i = 0; i < 8; i++) X[i] = X0[i];
            float Y[8] = {};    for (size_t i = 0; i < 8; i++) Y[i] = Y0[i];

            bool is_inside[8] = {true, true, true, true, true, true, true, true};
            for (size_t i = 0; i < MAX_DOT_INDEX; i++) {

                float x2[8] = {};   for (size_t j = 0; j < 8; j++) x2[j] =  X[j] * X[j];
                float y2[8] = {};   for (size_t j = 0; j < 8; j++) y2[j] =  Y[j] * Y[j];
                float xy[8] = {};   for (size_t j = 0; j < 8; j++) xy[j] =  X[j] * Y[j];
                float r2[8] = {};   for (size_t j = 0; j < 8; j++) r2[j] = x2[j] + y2[j];

                for (size_t j = 0; j < 8; j++) {
                    if (!is_inside[j]) continue;
                    is_inside[j] = r2[j] <= MAX_RADIUS_SQUARE;
                }

                for (size_t j = 0; j < 8; j++)  X[j] = x2[j] - y2[j] + X0[j];
                for (size_t j = 0; j < 8; j++)  Y[j] = xy[j] + xy[j] + Y0[j];
            }

            for (unsigned int i = 0; i < 8; i++) {
                size_t index = y_index * 800 + x_index + i;
                pixels[index].position = sf::Vector2f((float) (x_index + i),(float) y_index);
                if (is_inside[i])   pixels[index].color = sf::Color::Black;
                else                pixels[index].color = sf::Color::White;
            }
        }
    }

    #ifdef MEASURE
    unsigned long long end = __rdtsc();
    fprintf(fn, "%llu\n", end - start);
    #endif
}
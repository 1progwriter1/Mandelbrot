#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "mandelbrot_data.h"
#include "draw_func.h"
#include <assert.h>


static void SetPixels(sf::VertexArray &pixels, WindowData *data);

int main() {

    WindowData data = {};
    SetWindowData(&data);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mandelbrot");
    sf::VertexArray pixels(sf::Points, 800 * 600);

    while (window.isOpen()) {

        ProceedKeyStrokes(window, &data);

        SetPixels(pixels, &data);

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.display();
    }

    return 0;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data) {

    assert(data);

    float scale_ratio = (float) data->height / (float) data->width;

    for (unsigned int y_index = 0; y_index < 600; y_index++) {

        float x_0 = (-((float) data->width) / 2) * data->dx * data->scale + data->offset_x;
        float y_0 = (((float) y_index) - (float) data->height / 2) * data->dy * data->scale * scale_ratio + data->offset_y;

        for (unsigned int x_index = 0; x_index < 800; x_index += 8, x_0 += data->dx * 8) {

            float X0[8] = {x_0, x_0 + data->dx, x_0 + data->dx * 2, x_0 + data->dx * 3, x_0 + data->dx * 4,
                           x_0 + data->dx * 5, x_0 + data->dx * 6, x_0 + data->dx * 7};
            float Y0[8] = {y_0, y_0 + data->dy, y_0 + data->dy * 2, y_0 + data->dy * 3, y_0 + data->dy * 4,
                           y_0 + data->dy * 5, y_0 + data->dy * 6, y_0 + data->dy * 7};

            float X[8] = {};    for (size_t i = 0; i < 8; i++) X[i] = X0[i];
            float Y[8] = {};    for (size_t i = 0; i < 8; i++) Y[i] = Y0[i];

            size_t dot_index[8] = {};
            bool is_inside = true;
            for (size_t i = 0; i < MAX_DOT_INDEX && is_inside; i++) {

                float x2[8] = {};   for (size_t j = 0; j < 8; j++) x2[j] =  X[j] * X[j];
                float y2[8] = {};   for (size_t j = 0; j < 8; j++) y2[j] =  Y[j] * Y[j];
                float xy[8] = {};   for (size_t j = 0; j < 8; j++) xy[j] =  X[j] * Y[j];
                float r2[8] = {};   for (size_t j = 0; j < 8; j++) r2[j] = x2[j] + y2[j];

                is_inside = false;
                for (size_t j = 0; j < 8; j++)
                    if (r2[j] <= MAX_RADIUS_SQUARE) {
                        dot_index[j]++;
                        is_inside = true;
                    }

                for (size_t j = 0; j < 8; j++)  X[j] = x2[j] - y2[j] + X0[j];
                for (size_t j = 0; j < 8; j++)  Y[j] = xy[j] + xy[j] + Y0[j];
            }

            for (unsigned int i = 0; i < 8; i++) {
                size_t index = y_index * 800 + x_index + i;
                pixels[index].position = sf::Vector2f((float) (x_index + i),(float) y_index);
                if (dot_index[i] < MAX_DOT_INDEX)   pixels[index].color = sf::Color::Black;
                else                                pixels[index].color = sf::Color::White;
            }
        }
    }
}
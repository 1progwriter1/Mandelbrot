#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "mandelbrot_data.h"
#include "draw_func.h"
#include <assert.h>

static void SetPixels(sf::VertexArray &pixels, WindowData *data);

int main() {

    WindowData data = {};
    SetWindowData(&data);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot");
    sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);

    while (window.isOpen()) {

        ProceedKeyStrokes(window, &data);
        if (!window.isOpen())   break;

        SetPixels(pixels, &data);

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.display();
    }

    return 0;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data) {

    assert(data);

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    for (unsigned int y_index = 0; y_index < data->height; y_index++) {

        float x_0 = (-((float) data->width) / 2) * dx + data->offset_x * data->scale;
        float y_0 = (((float) y_index) - (float) data->height / 2) * dy + data->offset_y * data->scale * data->scale_ratio;

        for (unsigned int x_index = 0; x_index < data->width; x_index++, x_0 += dx) {

            float x = x_0;
            float y = y_0;

            bool is_inside = true;
            for (size_t i = 0; i < MAX_DOT_INDEX; i++) {

                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;
                float radius_square = x2 + y2;

                if (radius_square >= MAX_RADIUS_SQUARE) {
                    is_inside = false;
                    break;
                }

                x = x2 - y2 + x_0;
                y = xy + xy + y_0;
            }

            size_t index = y_index * data->width + x_index;
            pixels[index].position = sf::Vector2f((float) x_index, (float) y_index);
            if (is_inside)  pixels[index].color = sf::Color::Black;
            else            pixels[index].color = sf::Color::White;
        }
    }

}
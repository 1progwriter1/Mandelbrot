#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "draw_func.h"
#include <assert.h>

void SetWindowData(WindowData *data) {

    assert(data);

    data->height    = SCREEN_HEIGHT;
    data->width     = SCREEN_WIDTH;

    data->dx        = STEP_X;
    data->dy        = STEP_Y;

    data->scale     = 1.f;
    data->offset_x  = 0.f;
    data->offset_y  = 0.f;

}

void ProceedKeyStrokes(sf::RenderWindow &window, WindowData *data) {

    assert(data);

    sf::Event event;
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Left)   data->offset_x += data->dx * 10.f;
                if (event.key.code == sf::Keyboard::Right)  data->offset_x -= data->dx * 10.f;
                if (event.key.code == sf::Keyboard::Up)     data->offset_y += data->dy * 10.f;
                if (event.key.code == sf::Keyboard::Down)   data->offset_y -= data->dy * 10.f;
                if (event.key.code == sf::Keyboard::A)      data->scale    += data->dx * 10.f;
                if (event.key.code == sf::Keyboard::Z)      data->scale    -= data->dx * 10.f;
            }
    }
}
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

    data->scale_ratio = (float) data->height / (float) data->width;

}

void ProceedKeyStrokes(sf::RenderWindow &window, WindowData *data) {

    assert(data);

    sf::Event event;
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Left)   data->offset_x += data->dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if (event.key.code == sf::Keyboard::Right)  data->offset_x -= data->dx * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if (event.key.code == sf::Keyboard::Up)     data->offset_y += data->dy * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if (event.key.code == sf::Keyboard::Down)   data->offset_y -= data->dy * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);

                if (event.key.code == sf::Keyboard::Z) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                        data->scale -= data->dx * ((data->scale > data->dx * 100.f) ? 100.f : 0);
                    else
                        data->scale -= data->dx * ((data->scale > data->dx *  10.f) ?  10.f : 0);
                }
                if (event.key.code == sf::Keyboard::A) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                        data->scale += data->dx * ((data->scale > data->dx * 100.f) ? 100.f : 0);
                    else
                        data->scale += data->dx * ((data->scale > data->dx *  10.f) ?  10.f : 0);
                }
            }
    }
}
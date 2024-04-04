#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "../headers/draw_func.h"
#include <assert.h>
#include <string.h>
#include <immintrin.h>
#include "../headers/mandelbrot_data.h"
#include <math.h>

void SetWindowData(WindowData *data) {

    assert(data);

    data->dx        = STEP_X;
    data->dy        = STEP_Y;

    data->scale     = 1.f;
    data->offset_x  = 0.f;
    data->offset_y  = 0.f;

    data->scale_ratio = (float) SCREEN_HEIGHT / (float) SCREEN_WIDTH;

    data->fps_data.font.loadFromFile(FONT_FILE);
    data->fps_data.text.setFont(data->fps_data.font);
    data->fps_data.text.setFillColor(sf::Color::White);
    data->fps_data.text.setCharacterSize(20);
}

void ProceedKeyStrokes(sf::RenderWindow &window, WindowData *data) {

    assert(data);

    sf::Event event;
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Left)
                    data->offset_x += data->dx * data->scale * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if (event.key.code == sf::Keyboard::Right)
                    data->offset_x -= data->dx * data->scale * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if (event.key.code == sf::Keyboard::Up)
                    data->offset_y += data->dy * data->scale * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);
                if (event.key.code == sf::Keyboard::Down)
                    data->offset_y -= data->dy * data->scale * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 100.f : 10.f);

                if (event.key.code == sf::Keyboard::Z)  data->scale /= sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? BIG_ZOOM : DEFAULT_ZOOM;
                if (event.key.code == sf::Keyboard::A)  data->scale *= sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? BIG_ZOOM : DEFAULT_ZOOM;
            }
    }
}

bool IfMeasure(const int argc, const char *argv[]) {

    assert(argv);

    if (argc < 2)
        return false;

    return strncmp(argv[1], "measure", sizeof("measure")) == 0;
}

float GetFps(WindowData *data) {

    assert(data);

    return 1 / data->fps_data.clock.restart().asSeconds();

}

void DrawWindow(sf::RenderWindow &window, sf::VertexArray &pixels, WindowData *data) {

    assert(data);

    char buffer[FPS_BUFFER_LEN] = {};
    snprintf(buffer, FPS_BUFFER_LEN - 1, "fps: %d", (int) round(GetFps(data)));

    data->fps_data.text.setString(buffer);

    window.clear(sf::Color::Black);
    window.draw(pixels);
    window.draw(data->fps_data.text);
    window.display();

}
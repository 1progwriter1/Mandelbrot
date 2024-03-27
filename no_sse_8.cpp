#include <SFML/Graphics.hpp>
#include <stdio.h>

const size_t MAX_DOT_INDEX = 100;
const float MAX_RADIUS_SQUARE = 10.f;
const float dx = 1/200.f;
const float dy = 1/150.f;

int main() {

    const sf::Color colors[4] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow};

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mandelbrot");
    sf::Texture texture = {};
    sf::Sprite sprite = {};
    sf::Image image = {};
    image.create(800, 600);

    float xOffset = 0.f;
    float yOffset = 0.f;
    float x_scale = 1.f;
    float y_scale = 1.f;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Left)   xOffset += dx * 10.f;
                if (event.key.code == sf::Keyboard::Right)  xOffset -= dx * 10.f;
                if (event.key.code == sf::Keyboard::Up)     yOffset += dy * 10.f;
                if (event.key.code == sf::Keyboard::Down)   yOffset -= dy * 10.f;
                if (event.key.code == sf::Keyboard::A)     {x_scale += dx * 10.f; y_scale += dy * 10.f;}
                if (event.key.code == sf::Keyboard::Z)     {x_scale -= dx * 10.f; y_scale -= dy * 10.f;}
            }

        }

        for (unsigned int y_index = 0; y_index < 600; y_index++) {

            float x_0 = ((-400.f) * dx + xOffset) * x_scale * 800 / 600;
            float y_0 = (((float) y_index - 300.f) * dy + yOffset) * y_scale;

            for (unsigned int x_index = 0; x_index < 800; x_index += 8, x_0 += dx * 8) {

                float X0[8] = {x_0, x_0 + dx, x_0 + dx * 2, x_0 + dx * 3, x_0 + dx * 4, x_0 + dx * 5, x_0 + dx * 6, x_0 + dx * 7};
                float Y0[8] = {y_0, y_0 + dy, y_0 + dy * 2, y_0 + dy * 3, y_0 + dy * 4, y_0 + dy * 5, y_0 + dy * 6, y_0 + dy * 7};

                float X[8] = {};    for (size_t i = 0; i < 8; i++) X[i] = X0[i];
                float Y[8] = {};    for (size_t i = 0; i < 8; i++) Y[i] = Y0[i];

                bool is_inside[8] = {true, true, true, true, true, true, true, true};
                for (size_t i = 0; i < MAX_DOT_INDEX; i++) {

                    float x2[8] = {};   for (size_t j = 0; j < 8; j++) x2[j] = X[j] * X[j];
                    float y2[8] = {};   for (size_t j = 0; j < 8; j++) y2[j] = Y[j] * Y[j];
                    float xy[8] = {};   for (size_t j = 0; j < 8; j++) xy[j] = X[j] * Y[j];
                    float r2[8] = {};   for (size_t j = 0; j < 8; j++) r2[j] = x2[j] + y2[j];

                    for (size_t j = 0; j < 8; j++) {
                        if (!is_inside[j])  continue;
                        is_inside[j] = r2[j] <= MAX_RADIUS_SQUARE;
                    }

                    for (size_t j = 0; j < 8; j++) X[j] = x2[j] - y2[j] + X0[j];
                    for (size_t j = 0; j < 8; j++) Y[j] = xy[j] + xy[j] + Y0[j];
                }

                for (unsigned int i = 0; i < 8; i++) {
                    if (is_inside[i]) image.setPixel(x_index + i, y_index, sf::Color::Black);
                    else image.setPixel(x_index + i, y_index, sf::Color::White);
                }
            }
        }

        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setPosition(0, 0);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <stdio.h>

const size_t MAX_DOT_INDEX = 100;
const float MAX_RADIUS_SQUARE = 10.f;

int main() {

    const sf::Color colors[4] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow};

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mandelbrot");
    sf::Texture texture = {};
    sf::Sprite sprite = {};
    sf::Image image = {};
    image.create(800, 600);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
        }

        float dx = 1/200.f;
        float dy = 1/150.f;

        for (unsigned int y_index = 0; y_index < 600; y_index++) {

            float x_0 = (-400.f) * dx;
            float y_0 = ((float) y_index - 300.f) * dy;

            for (unsigned int x_index = 0; x_index < 800; x_index++, x_0 += dx) {

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

                if (is_inside) image.setPixel(x_index, y_index, sf::Color::Black);
                else image.setPixel(x_index, y_index, sf::Color::White);
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
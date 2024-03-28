#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <emmintrin.h>
#include <immintrin.h>

const size_t MAX_DOT_INDEX = 100;
const float dx = 1/200.f;
const float dy = 1/150.f;

const __m256 _76543210 = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
const __m256 MAX_RADIUS_SQUARE = _mm256_set1_ps(100.f);

int main() {

    const sf::Color colors[4] = {sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow};

    sf::RenderWindow window(sf::VideoMode(800, 600), "Mandelbrot");
    sf::VertexArray pixels(sf::Points, 800 * 600);

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

                __m256 X0 = _mm256_add_ps(_mm256_set1_ps(x_0), _mm256_mul_ps(_76543210, _mm256_set1_ps(dx)));
                __m256 Y0 = _mm256_set1_ps(y_0);

                __m256 X = X0;
                __m256 Y = Y0;

                __m256 is_inside = _mm256_set1_ps(0);
                __m256i cur_dot = _mm256_set1_epi32(0);

                for (size_t i = 0; i < MAX_DOT_INDEX; i++) {

                    __m256 x2 = _mm256_mul_ps(X, X);
                    __m256 y2 = _mm256_mul_ps(Y, Y);
                    __m256 xy = _mm256_mul_ps(X, Y);
                    __m256 r2 = _mm256_add_ps(x2, y2);

                    is_inside = _mm256_cmp_ps(r2, MAX_RADIUS_SQUARE, _CMP_LE_OQ);

                    int mask = _mm256_movemask_ps(is_inside);
                    if (!mask) break;

                    X = _mm256_add_ps(_mm256_sub_ps(x2, y2), X0);
                    Y = _mm256_add_ps(_mm256_add_ps(xy, xy), Y0);

                    cur_dot = _mm256_add_epi32(cur_dot, _mm256_cvtps_epi32(is_inside));
                }

                int *pn = (int *) &cur_dot;
                for (unsigned int i = 0; i < 8; i++) {
                    size_t index = y_index * 800 + x_index + i;
                    pixels[index].position = sf::Vector2f(x_index + i, y_index);
                    if ((size_t) pn[i] < MAX_DOT_INDEX) pixels[y_index * 800 + x_index + i].color = sf::Color::Black;
                    else                                pixels[y_index * 800 + x_index + i].color = sf::Color::White;
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <emmintrin.h>
#include <immintrin.h>
#include "mandelbrot_data.h"
#include "draw_func.h"
#include <assert.h>

const __m256 _76543210 = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
const __m256 MAX_RADIUS_SQUARE_SIMD = _mm256_set1_ps(100.f);

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

            __m256 X0 = _mm256_add_ps(_mm256_set1_ps(x_0), _mm256_mul_ps(_76543210, _mm256_set1_ps(data->dx)));
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

                is_inside = _mm256_cmp_ps(r2, MAX_RADIUS_SQUARE_SIMD, _CMP_LE_OQ);

                int mask = _mm256_movemask_ps(is_inside);
                if (!mask) break;

                X = _mm256_add_ps(_mm256_sub_ps(x2, y2), X0);
                Y = _mm256_add_ps(_mm256_add_ps(xy, xy), Y0);

                cur_dot = _mm256_sub_epi32(cur_dot, _mm256_cvtps_epi32(is_inside));
            }

            int *pn = (int *) &cur_dot;
            for (unsigned int i = 0; i < 8; i++) {
                size_t index = y_index * 800 + x_index + i;
                pixels[index].position = sf::Vector2f((float) (x_index + i), (float) y_index);
                if ((size_t) pn[i] < MAX_DOT_INDEX) pixels[y_index * 800 + x_index + i].color = sf::Color::Black;
                else                                pixels[y_index * 800 + x_index + i].color = sf::Color::White;
            }

        }
    }

}
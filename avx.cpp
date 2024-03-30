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
static __m256i CalculateDots(float x0, float y0, float dx);

int main() {

    WindowData data = {};
    SetWindowData(&data);

    sf::VertexArray pixels(sf::Points, data.width * data.height);

   #ifndef MEASURE
    sf::RenderWindow window(sf::VideoMode(data.width, data.height), "Mandelbrot");

    while (window.isOpen()) {

        ProceedKeyStrokes(window, &data);

        SetPixels(pixels, &data);

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.display();
    }
    #else
    FILE *fn = fopen(AVX_FILE, "w");
    if (!fn)    return 1;

    for (size_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++) {

        unsigned long long start = __rdtsc();

        for (size_t j = 0; j < NUMBER_OF_SCREENS; j++)
            SetPixels(pixels, &data);

        unsigned long long end = __rdtsc();
        fprintf(fn, "%llu\n", end - start);
    }
    fclose(fn);
    #endif

    return 0;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data) {

    assert(data);

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    __m256 set_1;

    for (unsigned int y_index = 0; y_index < data->height; y_index++) {

        float x_0 = (-((float) data->width) / 2) * dx + data->offset_x * data->scale;
        float y_0 = (((float) y_index) - (float) data->height / 2) * dy + data->offset_y * data->scale * data->scale_ratio;

        for (unsigned int x_index = 0; x_index < data->width; x_index += 8, x_0 += dx * 8) {

            __m256i cur_dot_index = CalculateDots(x_0, y_0, dx);

            #ifndef MEASURE
            int *int_cur_dot = (int *) &cur_dot_index;
            for (unsigned int i = 0; i < 8; i++) {
                size_t index = y_index * 800 + x_index + i;
                pixels[index].position = sf::Vector2f((float) (x_index + i), (float) y_index);
                if ((size_t) int_cur_dot[i] >= MAX_DOT_INDEX)   pixels[y_index * 800 + x_index + i].color = sf::Color::Black;
                else                                            pixels[y_index * 800 + x_index + i].color = sf::Color(255 - (char) int_cur_dot[i],
                                                                                                                     (char) int_cur_dot[i] % 8 * 32,
                                                                                                                     (char) int_cur_dot[i]);
            }
            #endif

        }
    }
}

static __m256i CalculateDots(float x0, float y0, float dx) {

        __m256 X0 = _mm256_add_ps(_mm256_set1_ps(x0), _mm256_mul_ps(_76543210, _mm256_set1_ps(dx)));
        __m256 Y0 = _mm256_set1_ps(y0);

        __m256 X = X0;
        __m256 Y = Y0;

        __m256 is_inside = _mm256_set1_ps(0);
        __m256i dots_indexes = _mm256_set1_epi32(0);

        for (size_t i = 0; i < MAX_DOT_INDEX; i++) {

            __m256 x2 = _mm256_mul_ps(X, X);
            __m256 y2 = _mm256_mul_ps(Y, Y);
            __m256 r2 = _mm256_add_ps(x2, y2);

            is_inside = _mm256_cmp_ps(r2, MAX_RADIUS_SQUARE_SIMD, _CMP_LE_OQ);

            int mask = _mm256_movemask_ps(is_inside);
            if (!mask) break;

            dots_indexes = _mm256_add_epi32(dots_indexes, _mm256_cvtps_epi32(_mm256_and_ps(is_inside, _mm256_set1_ps(1.f))));

            __m256 xy = _mm256_mul_ps(X, Y);
            X = _mm256_add_ps(_mm256_sub_ps(x2, y2), X0);
            Y = _mm256_add_ps(_mm256_add_ps(xy, xy), Y0);
        }

        return dots_indexes;
}
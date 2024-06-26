#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <emmintrin.h>
#include <immintrin.h>
#include "../headers/mandelbrot_data.h"
#include "../headers/draw_func.h"
#include <assert.h>

const __m256 _76543210 = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
const __m256 MAX_RADIUS_SQUARE_SIMD = _mm256_set1_ps(100.f);

static int MeasureProgram(WindowData *data, const char *time_file, const char *ticks_file, float *test_sum);
static void SetPixels(sf::VertexArray &pixels, WindowData *data, bool if_measure);
static __m256i CalculateDots(float x0, float y0, float dx);

int main(const int argc, const char *argv[]) {

    assert(argv);

    WindowData data = {};
    SetWindowData(&data);

    if (!IfMeasure(argc, argv)) {
        sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);
        sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot");

        while (window.isOpen()) {

            ProceedKeyStrokes(window, &data);

            SetPixels(pixels, &data, false);

            DrawWindow(window, pixels, &data);
        }
    }
    else {
        float test_sum = 0;
        if (MeasureProgram(&data, AVX_TIME, AVX_TICKS, &test_sum) != SUCCESS)
            return ERROR;
        printf("test sum: %f\n", test_sum);
    }

    return 0;
}

static void SetPixels(sf::VertexArray &pixels, WindowData *data, bool if_measure) {

    assert(data);

    float dy = data->dy * data->scale * data->scale_ratio;
    float dx = data->dx * data->scale;

    for (unsigned int y_index = 0; y_index < SCREEN_HEIGHT; y_index++) {

        float x0 = (-((float) SCREEN_WIDTH) / 2) * dx + data->offset_x;
        float y0 = (((float) y_index) - (float) SCREEN_HEIGHT / 2) * dy + data->offset_y;

        for (unsigned int x_index = 0; x_index < SCREEN_WIDTH; x_index += 8, x0 += dx * 8) {

            __m256i cur_dot_index = CalculateDots(x0, y0, dx);

            if (if_measure) {
                pixels[y_index * SCREEN_WIDTH + x_index].position.x = dot_index;
                continue;
            }

            int *int_cur_dot = (int *) &cur_dot_index;
            for (unsigned int i = 0; i < 8; i++) {
                size_t index = y_index * 800 + x_index + i;
                pixels[index].position = sf::Vector2f((float) (x_index + i), (float) y_index);
                if ((size_t) int_cur_dot[i] >= MAX_DOT_INDEX)   pixels[y_index * 800 + x_index + i].color = sf::Color::Black;
                else                                            pixels[y_index * 800 + x_index + i].color = sf::Color(255 - (char) int_cur_dot[i],
                                                                                                                     (char) int_cur_dot[i] % 8 * 32,
                                                                                                                     (char) int_cur_dot[i]);
            }
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

static int MeasureProgram(WindowData *data, const char *time_file, const char *ticks_file, float *test_sum) {

    assert(data);
    assert(time_file);
    assert(ticks_file);
    assert(test_sum);

    FILE *time  = fopen(time_file, "w");
    if (!time)  return FILE_OPEN_ERROR;

    FILE *ticks = fopen(ticks_file, "w");
    if (!ticks) {
        fclose(time);
        return FILE_OPEN_ERROR;
    }

    float tmp_sum = 0;
    sf::VertexArray pixels(sf::Points, SCREEN_WIDTH * SCREEN_HEIGHT);
    for (size_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++) {

        printf("measurement №%lu\n", i + 1);

        time_t time_start = clock();
        unsigned long long start = __rdtsc();

        for (size_t j = 0; j < NUMBER_OF_SCREENS; j++) {
            SetPixels(pixels, data, true);
            tmp_sum += pixels[(unsigned int) rand() % SCREEN_HEIGHT * SCREEN_WIDTH].position.x;
        }

        unsigned long long end = __rdtsc();
        time_t time_end = clock();

        fprintf(ticks, "%llu\n", end - start);
        fprintf(time, "%f\n", (double) (time_end - time_start) / CLOCKS_PER_SEC);
    }

    fclose(ticks);
    fclose(time);

    *test_sum = tmp_sum;

    return SUCCESS;
}
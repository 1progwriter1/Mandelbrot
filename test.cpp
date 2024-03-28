#include <immintrin.h>
#include <stdio.h>

int main() {

    __m256 x = _mm256_set_ps(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
    __m256 y = _mm256_set_ps(9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f);
    __m256 sum = _mm256_add_ps(x, y);

    printf("ok");
}
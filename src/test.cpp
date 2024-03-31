#include <immintrin.h>
#include <emmintrin.h>
#include <stdio.h>

int main() {

    __m256 r2 = _mm256_set1_ps(23.f);
    __m256 r3 = _mm256_set1_ps(24.f);
    __m256 minus = _mm256_set1_ps(100.f);

    __m256i arr = _mm256_set1_epi32(0);
    __m256  cmp = _mm256_set1_ps(0);
    cmp = _mm256_cmp_ps(r2, r3, _CMP_LE_OQ);
    arr = _mm256_add_epi32(arr, _mm256_cvtps_epi32(_mm256_and_ps(cmp, _mm256_set1_ps(1.f))));

    int *arr2 = (int *) &arr;
    for (size_t i = 0; i < 8; i++)
        printf("%d ", arr2[i]);
    printf("\n");

    printf("ok");
}
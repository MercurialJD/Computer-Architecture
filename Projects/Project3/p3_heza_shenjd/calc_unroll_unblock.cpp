#include <omp.h>
#include <immintrin.h>
void calc (int n, const int *a, const int *b, int *c) {
    register int temp;
    omp_set_nested(1);
    #pragma omp parallel for schedule(dynamic, 1024) private(temp)
    for (int k = 0; k < n; ++k) {
        {
            temp = 0;
            register int i;
            __m256i sum1 = _mm256_setzero_si256(), sum2, sum3;

            for (i = 0; i < n - k - 31; i = i + 32) {
                __m256i *a256 = (__m256i *)(a+i+k);
                __m256i *b256 = (__m256i *)(b+i);

                __m256i av = _mm256_load_si256(a256);
                __m256i bv = _mm256_loadu_si256(b256);
                sum1 = _mm256_add_epi32(sum1, _mm256_mullo_epi16(av, bv));

                av = _mm256_load_si256(a256+1);
                bv = _mm256_loadu_si256(b256+1);
                sum1 = _mm256_add_epi32(sum1, _mm256_mullo_epi16(av, bv));

                av = _mm256_load_si256(a256+2);
                bv = _mm256_loadu_si256(b256+2);
                sum1 = _mm256_add_epi32(sum1, _mm256_mullo_epi16(av, bv));

                av = _mm256_load_si256(a256+3);
                bv = _mm256_loadu_si256(b256+3);
                sum1 = _mm256_add_epi32(sum1, _mm256_mullo_epi16(av, bv));
            }
        
            sum2 = _mm256_hadd_epi32(sum1, sum1);
            sum3 = _mm256_hadd_epi32(sum2, sum2);
            temp = ((int*)&sum3)[0] + ((int*)&sum3)[4];

            for (; i < n - k; ++i) {
                temp += *(a+i+k) * *(b+i);
            }
            *(c+k) = temp;
        }
    }
}

#include <omp.h>
#include <iostream>
#include <chrono>
#include <immintrin.h>
void
calc (int n, const int *a, const int *b, int *c)
{
    register int temp;
    short *aa = (short *)malloc(n*sizeof(short));
    short *bb = (short *)malloc(n*sizeof(short));

    #pragma omp parallel for schedule (dynamic, 1024)
    for (int i = 0; i < n; ++i) {
        aa[i] = short(a[i]);
        bb[i] = short(b[i]);
    }

    omp_set_nested(1);
    #pragma omp parallel for schedule (dynamic, 1024)
    for (int k = 0; k < n; ++k)
    {
        temp = 0;
        register int i;
        __m256i * vec_aa = (__m256i *)aa;
        __m256i * vec_bb = (__m256i *)bb;

        __m256i sum1 = _mm256_setzero_si256();
        __m256i sumTemp1 = _mm256_setzero_si256();
        __m256i sumTemp2 = _mm256_setzero_si256();
        for (i = 0; i < n - k - 127; i = i + 128) {
            if (!(i & 0b1111111)) {
                sumTemp1 = _mm256_add_epi32(sumTemp1,_mm256_cvtepu16_epi32(*((__m128i *)&sum1)));
                sumTemp2 = _mm256_add_epi32(sumTemp2,_mm256_cvtepu16_epi32(*(((__m128i *)&sum1)+1)));
                sum1 = _mm256_setzero_si256();
            }

            register int i_k = i + k;

            __m256i av1 = _mm256_load_si256((__m256i *)&aa[i_k]);
            __m256i bv1 = _mm256_loadu_si256((__m256i *)&bb[i]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av1, bv1));

            __m256i av2 = _mm256_load_si256((__m256i *)&aa[i_k+16]);
            __m256i bv2 = _mm256_loadu_si256((__m256i *)&bb[i+16]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av2, bv2));

            av1 = _mm256_loadu_si256((__m256i *)&aa[i_k+32]);
            bv1 = _mm256_loadu_si256((__m256i *)&bb[i+32]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av1, bv1));

            av2 = _mm256_loadu_si256((__m256i *)&aa[i_k+48]);
            bv2 = _mm256_loadu_si256((__m256i *)&bb[i+48]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av2, bv2));

            av1 = _mm256_loadu_si256((__m256i *)&aa[i_k+64]);
            bv1 = _mm256_loadu_si256((__m256i *)&bb[i+64]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av1, bv1));

            av2 = _mm256_loadu_si256((__m256i *)&aa[i_k+80]);
            bv2 = _mm256_loadu_si256((__m256i *)&bb[i+80]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av2, bv2));

            av1 = _mm256_loadu_si256((__m256i *)&aa[i_k+96]);
            bv1 = _mm256_loadu_si256((__m256i *)&bb[i+96]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av1, bv1));

            av2 = _mm256_loadu_si256((__m256i *)&aa[i_k+112]);
            bv2 = _mm256_loadu_si256((__m256i *)&bb[i+112]);
            sum1 = _mm256_add_epi16(sum1, _mm256_mullo_epi16(av2, bv2));
        }
        
        sumTemp1 = _mm256_add_epi32(sumTemp1, _mm256_cvtepu16_epi32(*((__m128i *)&sum1)));
        sumTemp2 = _mm256_add_epi32(sumTemp2, _mm256_cvtepu16_epi32(*(((__m128i *)&sum1)+1)));
        sum1 = _mm256_add_epi32(sumTemp1, sumTemp2);
        __m256i sum2 = _mm256_hadd_epi32(sum1, sum1);
        sum1 = _mm256_hadd_epi32(sum2, sum2);
        temp = ((int*)&sum1)[0] + ((int*)&sum1)[4];

        for (; i < n - k; ++i) {
            temp += a[i + k] * b[i];
        }
        c[k] = temp;
    }
    
    free(aa); free(bb);
}

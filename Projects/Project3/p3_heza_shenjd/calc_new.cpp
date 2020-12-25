#include <omp.h>
#include <iostream>
#include <immintrin.h>
#include <chrono>

#define BLOCK 4

void calc (int n, const int *a, const int *b, int *c) {
    int rem = n & 0b11;
    int N = (n < 16) ? 16 : n;
    short *aa = (short *)malloc(N*sizeof(n)/2);
    short *bb = (short *)malloc(N*sizeof(n)/2);
    int i;
    for (i = 0; i < n; ++i) {
        aa[i] = (short)a[i];
        bb[i] = (short)b[i];
    }
    for (; i < N; ++i) {
        aa[i] = 0;
        bb[i] = 0;
    }


    #pragma omp parallel for schedule (dynamic, 1024)
    for (int k = 0; k < n - (BLOCK - 1); k+=BLOCK) {
        register int temp = 0;

        __m256i b1_sum16 = _mm256_setzero_si256(); __m256i b1_sum32_t1 = _mm256_setzero_si256(); __m256i b1_sum32_t2 = _mm256_setzero_si256();

        __m256i b2_sum16 = _mm256_setzero_si256(); __m256i b2_sum32_t1 = _mm256_setzero_si256(); __m256i b2_sum32_t2 = _mm256_setzero_si256();

        __m256i b3_sum16 = _mm256_setzero_si256(); __m256i b3_sum32_t1 = _mm256_setzero_si256(); __m256i b3_sum32_t2 = _mm256_setzero_si256();

        __m256i b4_sum16 = _mm256_setzero_si256(); __m256i b4_sum32_t1 = _mm256_setzero_si256(); __m256i b4_sum32_t2 = _mm256_setzero_si256();

        __m256i b_sum2, b_sum3;

        int i;
        for (i = 0; i < n - k - 15; i = i + 16) {
            // Block 1
            {
                if (!(i & 0b111111)) {
                    b1_sum32_t1 = _mm256_add_epi32(b1_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b1_sum16)));
                    b1_sum32_t2 = _mm256_add_epi32(b1_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b1_sum16)+1)));
                    b1_sum16 = _mm256_setzero_si256();
                }
                __m256i av = _mm256_loadu_si256((__m256i *)&aa[i+k]);
                __m256i bv = _mm256_loadu_si256((__m256i *)&bb[i]);
                b1_sum16 = _mm256_add_epi32(b1_sum16, _mm256_mullo_epi16(av,bv));
            }
            
            // Block 2
            {
                if (!(i & 0b111111)) {
                    b2_sum32_t1 = _mm256_add_epi32(b2_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b2_sum16)));
                    b2_sum32_t2 = _mm256_add_epi32(b2_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b2_sum16)+1)));
                    b2_sum16 = _mm256_setzero_si256();
                }
                __m256i av = _mm256_loadu_si256((__m256i *)&aa[i+k+1]);
                __m256i bv = _mm256_loadu_si256((__m256i *)&bb[i]);
                b2_sum16 = _mm256_add_epi32(b2_sum16, _mm256_mullo_epi16(av,bv));
            }

            // Block 3
            {
                if (!(i & 0b111111)) {
                    b3_sum32_t1 = _mm256_add_epi32(b3_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b3_sum16)));
                    b3_sum32_t2 = _mm256_add_epi32(b3_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b3_sum16)+1)));
                    b3_sum16 = _mm256_setzero_si256();
                }
                __m256i av = _mm256_loadu_si256((__m256i *)&aa[i+k+2]);
                __m256i bv = _mm256_loadu_si256((__m256i *)&bb[i]);
                b3_sum16 = _mm256_add_epi32(b3_sum16, _mm256_mullo_epi16(av,bv));
            }

            // Block 4
            {
                if (!(i & 0b111111)) {
                    b4_sum32_t1 = _mm256_add_epi32(b4_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b4_sum16)));
                    b4_sum32_t2 = _mm256_add_epi32(b4_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b4_sum16)+1)));
                    b4_sum16 = _mm256_setzero_si256();
                }
                __m256i av = _mm256_loadu_si256((__m256i *)&aa[i+k+3]);
                __m256i bv = _mm256_loadu_si256((__m256i *)&bb[i]);
                b4_sum16 = _mm256_add_epi32(b4_sum16, _mm256_mullo_epi16(av,bv));
            }
        }

        int ii = i;

        // Block 1
        {
            b1_sum32_t1 = _mm256_add_epi32(b1_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b1_sum16)));
            b1_sum32_t2 = _mm256_add_epi32(b1_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b1_sum16)+1)));
            b1_sum16 = _mm256_add_epi32(b1_sum32_t1,b1_sum32_t2);

            __m256i b1_sum2 = _mm256_hadd_epi32(b1_sum16, b1_sum16);
            __m256i b1_sum3 = _mm256_hadd_epi32(b1_sum2, b1_sum2);
            temp = ((int*)&b1_sum3)[0]+((int*)&b1_sum3)[3];

            for (; ii < n - k; ++ii) {
                temp += a[ii + k] * b[ii];
            }
            c[k] = temp;
        }

        // Block 2
        ii = i;
        {
            b2_sum32_t1 = _mm256_add_epi32(b2_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b2_sum16)));
            b2_sum32_t2 = _mm256_add_epi32(b2_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b2_sum16)+1)));
            b2_sum16 = _mm256_add_epi32(b2_sum32_t1,b2_sum32_t2);

            b_sum2 = _mm256_hadd_epi32(b2_sum16, b2_sum16);
            b_sum3 = _mm256_hadd_epi32(b_sum2, b_sum2);
            temp = ((int*)&b_sum3)[0]+((int*)&b_sum3)[4];

            for (; ii < n - k - 1; ++ii) {
                temp += a[ii + k + 1] * b[ii];
            }
            c[k+1] = temp;
        }

        // Block 3
        ii = i;
        {
            b3_sum32_t1 = _mm256_add_epi32(b3_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b3_sum16)));
            b3_sum32_t2 = _mm256_add_epi32(b3_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b3_sum16)+1)));
            b3_sum16 = _mm256_add_epi32(b3_sum32_t1,b3_sum32_t2);
            
            b_sum2 = _mm256_hadd_epi32(b3_sum16, b3_sum16);
            b_sum3 = _mm256_hadd_epi32(b_sum2, b_sum2);
            temp = ((int*)&b_sum3)[0]+((int*)&b_sum3)[4];
            
            for (; ii < n - k - 2; ++ii) {
                temp += a[ii + k + 2] * b[ii];
            }
            c[k+2] = temp;
        }

        // Block 4
        ii = i;
        {
            b4_sum32_t1 = _mm256_add_epi32(b4_sum32_t1,_mm256_cvtepu16_epi32(*((__m128i *)&b4_sum16)));
            b4_sum32_t2 = _mm256_add_epi32(b4_sum32_t2,_mm256_cvtepu16_epi32(*(((__m128i *)&b4_sum16)+1)));
            b4_sum16 = _mm256_add_epi32(b4_sum32_t1,b4_sum32_t2);
            
            b_sum2 = _mm256_hadd_epi32(b4_sum16, b4_sum16);
            b_sum3 = _mm256_hadd_epi32(b_sum2, b_sum2);
            temp = ((int*)&b_sum3)[0]+((int*)&b_sum3)[4];

            for (; ii < n - k - 3; ++ii) {
                temp += a[ii + k + 3] * b[ii];
            }
            c[k+3] = temp;
        }

        // c[k] += a[i+k] * b[i] + a[i+k+1] * b[i+1] + a[i+k+2] * b[i+2];
        // c[k+1] += a[i+k+1] * b[i] + a[i+k+2] * b[i+1];
        // c[k+2] += a[i+k+2] * b[i];
        // for (int id = 0; id < n; ++id) {
        //     std::cout << id << ':' << c[id] << '\t';
        // }
        // std::cout << std::endl;
    }

    #pragma omp parallel for schedule (dynamic, 1024)
    for (int kk = n - rem; kk < n; ++kk){
        for (int i = 0; i < n - kk; ++i){
            c[kk] += a[i + kk] * b[i];
        }
        // for (int id = 0; id < n; ++id) {
        //     std::cout << id << ':' << c[id] << '\t';
        // }
        // std::cout << std::endl;
    }
}

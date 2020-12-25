#include <omp.h>
#include <iostream>
#include <immintrin.h>
void
calc (int n, const int *a, const int *b, int *c) {
    register int temp1, temp2, temp3, temp4;
    int block = 4;
    short *aa = (short *)malloc(n*sizeof(short));
    short *bb = (short *)malloc(n*sizeof(short));
    for (int i = 0; i < n; ++i) {
        aa[i] = short(a[i]);
        bb[i] = short(b[i]);
    }

    int k;
    omp_set_nested(1);
    #pragma omp parallel for schedule (dynamic, 1024)
    for (k = 0; k < n - block + 1; k+=block) {
        temp1 = temp2 = temp3 = temp4 = 0;
        int i;

        __m256i sum11 = _mm256_setzero_si256();
        __m256i sum21 = _mm256_setzero_si256();
        __m256i sum31 = _mm256_setzero_si256();
        __m256i sum41 = _mm256_setzero_si256();
        __m256i sumTemp11 = _mm256_setzero_si256();
        __m256i sumTemp12 = _mm256_setzero_si256();
        __m256i sumTemp21 = _mm256_setzero_si256();
        __m256i sumTemp22 = _mm256_setzero_si256();
        __m256i sumTemp31 = _mm256_setzero_si256();
        __m256i sumTemp32 = _mm256_setzero_si256();
        __m256i sumTemp41 = _mm256_setzero_si256();
        __m256i sumTemp42 = _mm256_setzero_si256();
        

        for (i = 0; i < n - k - 127 - block * 128; i = i + 128) {
            {
                if (!(i & 0b111111)) {
                    sumTemp11 = _mm256_add_epi32(sumTemp11, _mm256_cvtepu16_epi32(*((__m128i *)&sum11)));
                    sumTemp12 = _mm256_add_epi32(sumTemp12, _mm256_cvtepu16_epi32(*(((__m128i *)&sum11)+1)));
                    sum11 = _mm256_setzero_si256();
                }
                __m256i av1 = _mm256_load_si256((__m256i *)&aa[i+k]);
                __m256i bv1 = _mm256_loadu_si256((__m256i *)&bb[i]);
                sum11 = _mm256_add_epi32(sum11, _mm256_mullo_epi16(av1,bv1));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av1,bv1));

                __m256i av2 = _mm256_load_si256((__m256i *)&aa[i+k+16]);
                __m256i bv2 = _mm256_loadu_si256((__m256i *)&bb[i+16]);
                sum11 = _mm256_add_epi16(sum11, _mm256_mullo_epi16(av2,bv2));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+32]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+32]);
                sum11 = _mm256_add_epi16(sum11, _mm256_mullo_epi16(av1,bv1));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+48]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+48]);
                sum11 = _mm256_add_epi16(sum11, _mm256_mullo_epi16(av2,bv2));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+64]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+64]);
                sum11 = _mm256_add_epi16(sum11, _mm256_mullo_epi16(av1,bv1));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+80]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+80]);
                sum11 = _mm256_add_epi16(sum11, _mm256_mullo_epi16(av2,bv2));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+96]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+96]);
                sum11 = _mm256_add_epi16(sum11, _mm256_mullo_epi16(av1,bv1));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+112]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+112]);
                sum11 = _mm256_add_epi16(sum11, _mm256_mullo_epi16(av2,bv2));
                // sum11 = _mm256_add_epi32(sum11, _mm256_maddubs_epi16(av2,bv2));
            }
            {
                sumTemp11 = _mm256_add_epi32(sumTemp11, _mm256_cvtepu16_epi32(*((__m128i *)&sum11)));
                sumTemp12 = _mm256_add_epi32(sumTemp12, _mm256_cvtepu16_epi32(*(((__m128i *)&sum11)+1)));
                sum11 = _mm256_add_epi32(sumTemp11, sumTemp12);
                __m256i sum12 = _mm256_hadd_epi32(sum11, sum11);
                __m256i sum13 = _mm256_hadd_epi32(sum12, sum12);
                temp1 = ((int*)&sum13)[0]+((int*)&sum13)[4];
                for (; i < n - k; ++i) {
                    temp1 += a[i + k] * b[i];
                }
            }
            c[k] = temp1;

            {
                if (!(i & 0b111111)) {
                    sumTemp21 = _mm256_add_epi32(sumTemp21, _mm256_cvtepu16_epi32(*((__m128i *)&sum21)));
                    sumTemp22 = _mm256_add_epi32(sumTemp22, _mm256_cvtepu16_epi32(*(((__m128i *)&sum21)+1)));
                    sum21 = _mm256_setzero_si256();
                }
                __m256i av1 = _mm256_load_si256((__m256i *)&aa[i+k]);
                __m256i bv1 = _mm256_loadu_si256((__m256i *)&bb[i]);
                sum21 = _mm256_add_epi32(sum21, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                __m256i av2 = _mm256_load_si256((__m256i *)&aa[i+k+16]);
                __m256i bv2 = _mm256_loadu_si256((__m256i *)&bb[i+16]);
                sum21 = _mm256_add_epi16(sum21, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+32]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+32]);
                sum21 = _mm256_add_epi16(sum21, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+48]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+48]);
                sum21 = _mm256_add_epi16(sum21, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+64]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+64]);
                sum21 = _mm256_add_epi16(sum21, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+80]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+80]);
                sum21 = _mm256_add_epi16(sum21, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+96]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+96]);
                sum21 = _mm256_add_epi16(sum21, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+112]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+112]);
                sum21 = _mm256_add_epi16(sum21, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));
            }
            {
                sumTemp21 = _mm256_add_epi32(sumTemp21, _mm256_cvtepu16_epi32(*((__m128i *)&sum21)));
                sumTemp22 = _mm256_add_epi32(sumTemp22, _mm256_cvtepu16_epi32(*(((__m128i *)&sum21)+1)));
                sum21 = _mm256_add_epi32(sumTemp21, sumTemp22);
                __m256i sum22 = _mm256_hadd_epi32(sum21, sum21);
                __m256i sum23 = _mm256_hadd_epi32(sum22, sum22);
                temp2 = ((int*)&sum23)[0]+((int*)&sum23)[4];
                for (; i < n - k - 1; ++i) {
                    temp2 += a[i + k + 1] * b[i];
                }
            }
            c[k+1] = temp2;

            {
                if (!(i & 0b111111)) {
                    sumTemp31 = _mm256_add_epi32(sumTemp31, _mm256_cvtepu16_epi32(*((__m128i *)&sum31)));
                    sumTemp32 = _mm256_add_epi32(sumTemp32, _mm256_cvtepu16_epi32(*(((__m128i *)&sum31)+1)));
                    sum31 = _mm256_setzero_si256();
                }
                __m256i av1 = _mm256_load_si256((__m256i *)&aa[i+k]);
                __m256i bv1 = _mm256_loadu_si256((__m256i *)&bb[i]);
                sum31 = _mm256_add_epi32(sum31, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                __m256i av2 = _mm256_load_si256((__m256i *)&aa[i+k+16]);
                __m256i bv2 = _mm256_loadu_si256((__m256i *)&bb[i+16]);
                sum31 = _mm256_add_epi16(sum31, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+32]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+32]);
                sum31 = _mm256_add_epi16(sum31, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+48]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+48]);
                sum31 = _mm256_add_epi16(sum31, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+64]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+64]);
                sum31 = _mm256_add_epi16(sum31, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+80]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+80]);
                sum31 = _mm256_add_epi16(sum31, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+96]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+96]);
                sum31 = _mm256_add_epi16(sum31, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+112]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+112]);
                sum31 = _mm256_add_epi16(sum31, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));
            }
            {
                sumTemp31 = _mm256_add_epi32(sumTemp31, _mm256_cvtepu16_epi32(*((__m128i *)&sum31)));
                sumTemp32 = _mm256_add_epi32(sumTemp32, _mm256_cvtepu16_epi32(*(((__m128i *)&sum31)+1)));
                sum31 = _mm256_add_epi32(sumTemp31, sumTemp32);
                __m256i sum32 = _mm256_hadd_epi32(sum31, sum31);
                __m256i sum33 = _mm256_hadd_epi32(sum32, sum32);
                temp3 = ((int*)&sum33)[0]+((int*)&sum33)[4];
                for (; i < n - k - 2; ++i) {
                    temp3 += a[i + k + 2] * b[i];
                }
            }
            c[k+2] = temp3;

            {
                if (!(i & 0b111111)) {
                    sumTemp41 = _mm256_add_epi32(sumTemp41, _mm256_cvtepu16_epi32(*((__m128i *)&sum41)));
                    sumTemp42 = _mm256_add_epi32(sumTemp42, _mm256_cvtepu16_epi32(*(((__m128i *)&sum41)+1)));
                    sum41 = _mm256_setzero_si256();
                }
                __m256i av1 = _mm256_load_si256((__m256i *)&aa[i+k]);
                __m256i bv1 = _mm256_loadu_si256((__m256i *)&bb[i]);
                sum41 = _mm256_add_epi32(sum41, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                __m256i av2 = _mm256_load_si256((__m256i *)&aa[i+k+16]);
                __m256i bv2 = _mm256_loadu_si256((__m256i *)&bb[i+16]);
                sum41 = _mm256_add_epi16(sum41, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+32]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+32]);
                sum41 = _mm256_add_epi16(sum41, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+48]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+48]);
                sum41 = _mm256_add_epi16(sum41, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+64]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+64]);
                sum41 = _mm256_add_epi16(sum41, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+80]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+80]);
                sum41 = _mm256_add_epi16(sum41, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));

                av1 = _mm256_load_si256((__m256i *)&aa[i+k+96]);
                bv1 = _mm256_loadu_si256((__m256i *)&bb[i+96]);
                sum41 = _mm256_add_epi16(sum41, _mm256_mullo_epi16(av1,bv1));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av1,bv1));

                av2 = _mm256_load_si256((__m256i *)&aa[i+k+112]);
                bv2 = _mm256_loadu_si256((__m256i *)&bb[i+112]);
                sum41 = _mm256_add_epi16(sum41, _mm256_mullo_epi16(av2,bv2));
                // sum1 = _mm256_add_epi32(sum1, _mm256_maddubs_epi16(av2,bv2));
            }
            {
                sumTemp41 = _mm256_add_epi32(sumTemp41, _mm256_cvtepu16_epi32(*((__m128i *)&sum41)));
                sumTemp42 = _mm256_add_epi32(sumTemp42, _mm256_cvtepu16_epi32(*(((__m128i *)&sum41)+1)));
                sum41 = _mm256_add_epi32(sumTemp41, sumTemp42);
                __m256i sum42 = _mm256_hadd_epi32(sum41, sum41);
                __m256i sum43 = _mm256_hadd_epi32(sum42, sum42);
                temp4 = ((int*)&sum43)[0]+((int*)&sum43)[4];
                for (; i < n - k - 3; ++i) {
                    temp4 += a[i + k + 3] * b[i];
                }
            }
            c[k+3] = temp4;
            
            c[k] += a[i + k] * b[i]+a[i + k+1 ] * b[i+1] + a[i+k+2]*b[i+2];
            c[k+1]+=a[i + k+1 ] * b[i] + a[i+k+2]*b[i+1];
            c[k+2]+=a[i+k+2]*b[i];
        }
    }
    
    for (; k < n; k++){
        for (int i = 0; i < n - k; ++i){
            c[k] += a[i + k] * b[i];
        }
    }
}

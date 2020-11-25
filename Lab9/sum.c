#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h> /* where intrinsics are defined */


#define CLOCK_RATE_GHZ 2.0e9   /* just a wild guess ... */

/* Time stamp counter */
static __inline__ unsigned long long RDTSC(void) {
    unsigned hi,lo;
    __asm__ volatile("rdtsc" : "=a"(lo),"=d"(hi));
    return ((unsigned long long) lo)| (((unsigned long long)hi) << 32);
}

int sum_naive( int n, int *a )
{
    int sum = 0;
    for( int i = 0; i < n; i++ )
        sum += a[i];
    return sum;
}

int sum_unrolled( int n, int *a )
{
    int sum = 0;

    /* do the body of the work in a faster unrolled loop */
    for( int i = 0; i < n/4*4; i += 4 )
    {
        sum += a[i+0];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];
    }

    /* handle the small tail in a usual way */
    for( int i = n/4*4; i < n; i++ )   
        sum += a[i];

    return sum;
}

int sum_vectorized( int n, int *a )
{
    /* VECTORIZE YOUR CODE HERE*/
    int sum = 0;
    __m128i sum1  = _mm_setzero_si128();
    __m128i a1 = _mm_setzero_si128();
    for( int i = 0; i < n/4*4; i += 4)
    {
        a1 = _mm_loadu_si128(a+i);
        sum1 =  _mm_add_epi32(sum1 , a1);
    }

    sum = ((int*)&sum1)[0]+((int*)&sum1)[1]+((int*)&sum1)[2]+((int*)&sum1)[3];

    for(  int i = n/4*4; i < n; i++ )   
        sum += a[i];
    return sum;

    
}
int sum_vectorized_unrolled( int n, int *a )
{
    /* UNROLL YOUR VECTORIZED CODE HERE*/
    int sum = 0;
    __m128i sum1  = _mm_setzero_si128();
    __m128i a1 = _mm_setzero_si128();
    __m128i a2 = _mm_setzero_si128();
    __m128i a3 = _mm_setzero_si128();
    __m128i a4 = _mm_setzero_si128();

    for (int i = 0;i < n/16*16;i+=16)
    {
        a1 = _mm_loadu_si128(a+i);
        sum1 =  _mm_add_epi32(sum1 , a1);
        a2 = _mm_loadu_si128(a+i+4);
        sum1 =  _mm_add_epi32(sum1 , a2);
        a3 = _mm_loadu_si128(a+i+8);
        sum1 =  _mm_add_epi32(sum1 , a3);
        a4 = _mm_loadu_si128(a+i+12);
        sum1 =  _mm_add_epi32(sum1 , a4);
    }

    sum = ((int*)&sum1)[0]+((int*)&sum1)[1]+((int*)&sum1)[2]+((int*)&sum1)[3];
    
    for ( int i = n/16*16; i < n; i++)
        sum += a[i];

    return sum;
}

void benchmark( int n, int *a, int (*computeSum)(int,int*), char *name )
{
    /* warm up */
    int sum = computeSum( n, a );

    /* measure */
    unsigned long long cycles = RDTSC();
    sum += computeSum( n, a );
    cycles = RDTSC()-cycles;
    
    double microseconds = cycles/CLOCK_RATE_GHZ*1e6;
    
    /* report */
    printf( "%20s: ", name );
    if( sum == 2*sum_naive(n,a) ) 
        printf( "%.2f microseconds\n", microseconds,sum );
    else
        printf( "ERROR!\n",sum);
} 

int main( int argc, char **argv )
{
    const int n = 7777; /* small enough to fit in cache */
    
    /* init the array */
    int a[n] __attribute__ ((aligned (32))); /* align the array in memory by 32 bytes (good for 256 bit intrinsics) */
    for( int i = 0; i < n; i++ ) a[i] = rand( );
    
    /* benchmark series of codes */
    benchmark( n, a, sum_naive, "naive" );
    benchmark( n, a, sum_unrolled, "unrolled" );
    benchmark( n, a, sum_vectorized, "vectorized" );
    benchmark( n, a, sum_vectorized_unrolled, "vectorized unrolled" );

    return 0;
}

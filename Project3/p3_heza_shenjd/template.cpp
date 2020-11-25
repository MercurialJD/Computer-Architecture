void
calc (int n, const int *a, const int *b, int *c)
{
    int block = 4;
    int rem = n % block;
    
    for (int k = 0; k < n-block+1; k+=block) {
        int i=0;
        for ( ; i < n - k - block + 1; ++i){
            c[k] += a[i + k] * b[i];
            c[k+1] += a[i + k + 1] * b[i];
            c[k+2] += a[i + k + 2] * b[i]; 
            c[k+3] += a[i + k + 3] * b[i];
        }
        c[k] += a[i + k] * b[i]+a[i + k+1 ] * b[i+1] + a[i+k+2]*b[i+2];
        c[k+1]+=a[i + k+1 ] * b[i] + a[i+k+2]*b[i+1];
        c[k+2]+=a[i+k+2]*b[i];

    }
    for (int kk = n - rem; kk < n; ++kk){
        for (int i = 0; i < n - kk; ++i){
            c[kk] += a[i + kk] * b[i];
        }
    }
}

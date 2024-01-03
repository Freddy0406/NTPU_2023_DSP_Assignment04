# include "function.h"


/* hamming: for n=0,1,2,...N, length of N+1 */
float hamming(int N, int n)
{
	return 0.54 - 0.46 * cosf(2*PI*((float)(n))/((float)N));
}

/* low-pass filter coef: n=0,1,2...,2M */
float low_pass(int m, int n)
{
	float wc = 2*PI*FL/FS;
	if(n==m) {// L'Hopital's Rule
		return wc/PI;
	}
	else {
		return sinf(wc*((float)(n-m)))/PI/((float)(n-m)) * hamming(2*m+1, n);
	}
}



// void through_LPF(float *x, int lx, float *h, int lh, float *y)
// {
//     //int N = lx + lh - 1;
//     int i;
//     int k;
//     for(i=0;i<N;i++) {
//         /* flip x */
//         y[i] = 0.0f;
//         for(k=0;k<lh && (k<=i);k++) {
//             y[i] = y[i] + x[i-k] * h[k]; // flip x
//             //  x[i-k] would has some access violation for (i-k)>=lx
//         }
//     }
// }
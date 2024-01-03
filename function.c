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



void through_LPF(float *data, int data_length, float *h, int h_length, short *output){

    int n = 0;
    int k = 0;
    float y = 0.0;
    for(n=0;n<(data_length+h_length-1);n++) {
		y = 0.0;
		for(k=0;k<h_length;k++) {
			if( (n-k)>=0 && (n-k<data_length) )
		 		y = y + h[k] * data[n-k];
		}
		output[n] = (short)(roundf(L*y));			
		// output[n] = (short)(roundf(data[n]));
	}
	
}

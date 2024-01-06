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



void through_LPF(float *data, int N, float *h, short *output){

    int n = 0;
    int k = 0;
    float y = 0.0;
    for(n=0;n<(zp_N+P-1);n++) {
		y = 0.0;
		for(k=0;k<P;k++) {
			if( (n-k)>=0 && (n-k)<N)
		 		y = y + h[k] * data[n-k];
		}
		output[n] = (short)(roundf(y));			
	}
}

void overlap_add(short *in, short *out, short *pre){
	int i ;
    for(i = 0;i<(P-1);i++){				//Add previous array
        in[i] = in[i]+pre[i];
    }

	int index = 0;
    for(i=zp_N;i<(zp_N+P-1);i++){		//Copy last P-1 data to previous array
        pre[index] = in[i];
        index++;
    }

	int index2 = 0;						//down-sampling output
	for (i = 0;i<zp_N;i++){
		if((i%M)==0){
			out[index2] = in[i];
			index2++;
		}
	}
}

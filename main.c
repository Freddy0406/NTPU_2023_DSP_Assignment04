# include "function.h"



int main(int argc, char **argv){

    FILE *fp = fopen("input.wav", "rb");
    FILE *fp_out = fopen("output.raw", "wb");
    fseek(fp, 44, SEEK_SET);                                                        //跳過wav標頭
    short *data_read = (short*)malloc(sizeof(short)*(data_L));                      //讀取wav雙聲道        
    short *data_read_L = (short*)malloc(sizeof(short)*(data_L/2));                  //存取wav左聲道
    short *data_read_R = (short*)malloc(sizeof(short)*(data_L/2));                  //存取wav右聲道
    float *data_zp_L = (float*)malloc(sizeof(float)*(data_L+zp_N));                 //zero padding 左聲道
    float *data_zp_R = (float*)malloc(sizeof(float)*(data_L+zp_N));                 //zero padding 右聲道


    int n,i;
    int m = 0;  



    float *h = (float*)malloc(sizeof(float)*P);                         //128-Order LPF
    float *h_zp = (float*)malloc(sizeof(float)*(data_L+zp_N));          //zero padding of LPF
    for(n=0;n<P;n++) {
        h[n] = low_pass(MOrder, n);
    }


    // zero padding for h
    for(i=0;i<P;i++) {
        h_zp[i] = h[i];
    }
    for(i=P;i<(data_L+zp_N);i++) {
        h_zp[i] = 0.0f;
    }


while( fread(data_read, sizeof(short), data_L, fp) ) {
        // convolution y_m = x_m * h
        // convert data type
        for(i=0;i<(data_L-1);i+=2) {
            data_zp_L[i] = (float) data_read[i];
            data_zp_R[i+1] = (float) data_read[i+1];
        } 
        for(i=(data_L/2);i<(data_L+zp_N-1);i++) {
            data_zp_L[i] = 0.0f;
            data_zp_R[i] = 0.0f;
        }


        /* convolution in time domain */
        //conv(fx_m, L, h, P, y_m);


        // overlap and add
        // for(i=0;i<(L+P-1);i++) {
        //     y[m*L+i] = y[m*L+i] + y_m_zero_padded[i];            
        // }

        // // output waveform for the m-th window
        // for(i=0;i<L;i++) {
        //     s_y_m[i] = (short)roundf(y[m*L+i]);
        // }

        
        // for(i=0;i<L;i+=2) {
        //     fwrite(s_y_m+i, sizeof(short), 1, fp_out);
        // }
        
        m++;
    }













    free(data_read);
    free(data_read_L);
    free(data_read_R);
    free(data_zp_L);
    free(data_zp_R);
    free(h);
    free(h_zp);
    fclose(fp);
    fclose(fp_out);
    return 0;
}
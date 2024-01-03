# include "function.h"



int main(int argc, char **argv){

    FILE *fp = fopen("test.wav", "rb");
    FILE *fp_out = fopen("output.raw", "wb");
    fseek(fp, 44, SEEK_SET);                                                        //跳過wav標頭
    short *data_read = (short*)malloc(sizeof(short)*(data_L));                      //讀取wav雙聲道        
    short *data_read_L = (short*)malloc(sizeof(short)*(data_L/2));                  //存取wav左聲道
    short *data_read_R = (short*)malloc(sizeof(short)*(data_L/2));                  //存取wav右聲道
    float *data_zp_L = (float*)calloc((zp_N),sizeof(float));                 //zero padding 左聲道
    float *data_zp_R = (float*)calloc((zp_N),sizeof(float));                 //zero padding 右聲道

    short *output_L = (short*)calloc((data_L+zp_N+P-1),sizeof(short));                 //zero padding 左聲道
    short *output_R = (short*)calloc((data_L+zp_N+P-1),sizeof(short));                 //zero padding 左聲道



    int n,i;
    int m = 0;  



    float *h = (float*)malloc(sizeof(float)*P);                         //128-Order LPF
    for(n=0;n<P;n++) {
        h[n] = low_pass(MOrder, n);
    }

    int times = 0;

while( fread(data_read, sizeof(short), data_L, fp) ) {
        // convert data type
        for(i=0;i<(data_L-1);i+=2) {

            data_zp_L[i*L] = (float) data_read[i];
            data_zp_R[i*L] = (float) data_read[i+1];
        }


        // through_LPF(data_zp_L, (data_L+zp_N) , h, P, output_L);
        // through_LPF(data_zp_R, (data_L+zp_N) , h, P, output_R);


        // // overlap and add
        // // for(i=0;i<(L+P-1);i++) {
        // //     y[m*L+i] = y[m*L+i] + y_m_zero_padded[i];            
        // // }

        // // // output waveform for the m-th window
        // // for(i=0;i<L;i++) {
        // //     s_y_m[i] = (short)roundf(y[m*L+i]);
        // // }

        
        // for(i=0;i<(data_L+zp_N);i++) {
        //     fwrite(output_L+i, sizeof(short), 1, fp_out);
        //     fwrite(output_R+i, sizeof(short), 1, fp_out);
        // }
        // // m++;

        times++;
    }













    free(data_read);
    free(data_read_L);
    free(data_read_R);
    free(data_zp_L);
    free(data_zp_R);
    free(output_L);
    free(output_R);
    free(h);
    fclose(fp);
    fclose(fp_out);
    return 0;
}
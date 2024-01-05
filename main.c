# include "function.h"



int main(int argc, char **argv){

    FILE *fp = fopen("test.wav", "rb");
    FILE *fp_out = fopen("output.raw", "wb");
    struct WaveHeader Wavin_head;

    fread(&Wavin_head,sizeof(Wavin_head),1,fp);

    printf("%d\n",Wavin_head.DATALen);
    printf("wav length(secs) = %d\n",Wavin_head.DATALen*8/(Wavin_head.sample_bits*Wavin_head.channels*Wavin_head.samplehz));


    short *data_read = (short*)malloc(sizeof(short)*(data_L));                      //讀取wav雙聲道        
    float *data_zp_L = (float*)calloc((zp_N),sizeof(float));                 //zero padding 左聲道
    float *data_zp_R = (float*)calloc((zp_N),sizeof(float));                 //zero padding 右聲道
    short *lpf_L = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding 左聲道
    short *lpf_R = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding 左聲道


    short *out_L = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding 左聲道
    short *out_R = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding 左聲道


    int n,i;
    int m = 0;  



    FILE *fptxt  = fopen("h.txt","w+");
    float *h = (float*)malloc(sizeof(float)*P);                         //128-Order LPF
    for(n=0;n<P;n++) {
        h[n] = low_pass(MOrder, n);
        fprintf(fptxt,"%lf\n",h[n]);                                           //write into txt
        fflush(fptxt); 
    }
    fclose(fptxt);

    int times = 0;
    printf("%d\n",zp_N+P-1);

while( fread(data_read, sizeof(short), data_L, fp) ) {
        // convert data type


        for(i=0;i<(data_L-1);i+=2) {
            data_zp_L[(i*L)/2] = (float) data_read[i];
            data_zp_R[(i*L)/2] = (float) data_read[i+1];
        }


        // if(times==2){









        // if(times==0){
        //     for(i = 0;i<zp_N;i++){
        //         if(data_zp_R[i]!=0){
        //             printf("%d\n",i);
        //         }
        //     }
        // }




        // through_LPF(data_zp_L, zp_N , h,  lpf_L);
        // through_LPF(data_zp_R, zp_N , h,  lpf_R);






        // // overlap and add
        // // for(i=0;i<(L+P-1);i++) {
        // //     y[m*L+i] = y[m*L+i] + y_m_zero_padded[i];            
        // // }

        // // // output waveform for the m-th window
        // // for(i=0;i<L;i++) {
        // //     s_y_m[i] = (short)roundf(y[m*L+i]);
        // // }

        // for(i = 0;i<zp_N;i++){
        //     output_L[i] = (short)(data_zp_L[i]);
        //     output_R[i] = (short)(data_zp_R[i]);
        // }

        
        // for(i=0;i<(zp_N+P-1);i+=M) {
        //     fwrite(lpf_L+i, sizeof(short), 1, fp_out);
        //     fwrite(lpf_R+i, sizeof(short), 1, fp_out);
        // }
        // // m++;
        times++;
    }













    free(data_read);
    free(data_zp_L);
    free(data_zp_R);
    // free(output_L);
    // free(output_R);
    free(h);
    fclose(fp);
    fclose(fp_out);

    return 0;
}
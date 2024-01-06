# include "function.h"

int main(int argc, char **argv){

    FILE *fp = fopen("test.wav", "rb");
    FILE *fp_out = fopen("output.wav", "wb");
    struct WaveHeader Wavin_head;
    struct WaveHeader Wavout_head;
    fread(&Wavin_head,sizeof(Wavin_head),1,fp);
    int wav_time = Wavin_head.DATALen/Wavin_head.bytepsec;

    short *data_read = (short*)malloc(sizeof(short)*(data_L));               //讀取wav雙聲道        
    float *data_zp_L = (float*)calloc((zp_N),sizeof(float));                 //zero padding 左聲道
    float *data_zp_R = (float*)calloc((zp_N),sizeof(float));                 //zero padding 右聲道
    short *lpf_L = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding through lpf 左聲道
    short *lpf_R = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding through lpf 右聲道
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


    /*Make output header*/
    Wavout_head = Wavin_head;
    Wavout_head.samplehz = FS_out;
	Wavout_head.bytepsec = FS_out*Wavout_head.channels*(Wavout_head.sample_bits/8);
	Wavout_head.DATALen = Wavout_head.bytepsec*wav_time;
	Wavout_head.total_Len = 36+Wavout_head.DATALen;
    printf("out %d\n",Wavout_head.samplehz);
    fwrite(&Wavout_head,sizeof(Wavout_head),1,fp_out);
    short *yl;
    short *yr;
    short *out_L = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding through lpf 左聲道
    short *out_R = (short*)calloc((zp_N+P-1),sizeof(short));                 //zero padding through lpf 右聲道




while( fread(data_read, sizeof(short), data_L, fp) ) {
        // convert data type
        for(i=0;i<(data_L);i+=2) {
            data_zp_L[(i*L)/2] = (float) data_read[i];
            data_zp_R[(i*L)/2] = (float) data_read[i+1];
        }



        through_LPF(data_zp_L, zp_N , h,  lpf_L);
        through_LPF(data_zp_R, zp_N , h,  lpf_R);

        // if(times==0){
        //     for(i = 0;i<(zp_N);i++){
        //         if(lpf_L[i]!=0){
        //             printf("%d\n",i);
        //         }
        //     }
        // }

        // overlap and add
        // for(i=0;i<(zp_N+P-1);i++) {
        //     yl[m*(data_L/2)+i] = yl[m*(data_L/2)+i] + lpf_L[i];
        //     yr[m*(data_L/2)+i] = yr[m*(data_L/2)+i] + lpf_R[i];             
        // }

        // for(i=0;i<(zp_N);i++) {
        //     out_L[i] = yl[m*(data_L/2)+i];
        //     out_R[i] = yr[m*(data_L/2)+i];             
        // }

        
        // for(i=0;i<(zp_N);i+=M) {
        //     fwrite(out_L+i, sizeof(short), 1, fp_out);
        //     fwrite(out_R+i, sizeof(short), 1, fp_out);
        // }



        // for(i=0;i<(zp_N);i+=L) {
        //     fwrite(lpf_L+i, sizeof(short), 1, fp_out);
        //     fwrite(lpf_R+i, sizeof(short), 1, fp_out);
        // }
        m++;
        times++;
    }

    free(data_read);
    free(data_zp_L);
    free(data_zp_R);
    free(lpf_L);
    free(lpf_R);
    free(h);
    free(yl);
    free(yr);
    free(out_L);
    free(out_R);
    fclose(fp);
    fclose(fp_out);

    return 0;
}
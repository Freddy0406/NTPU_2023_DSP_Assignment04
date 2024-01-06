# include "function.h"

int main(){

    FILE *fp = fopen("input.wav", "rb");
    FILE *fp_out = fopen("output.wav", "wb");
    struct WaveHeader Wavin_head;
    struct WaveHeader Wavout_head;
    fread(&Wavin_head,sizeof(Wavin_head),1,fp);
    int wav_time = Wavin_head.DATALen/Wavin_head.bytepsec;

    short *data_read = (short*)malloc(sizeof(short)*(data_L));              //讀取wav雙聲道        
    float *data_zp_L = (float*)calloc((zp_N),sizeof(float));                //zero padding 左聲道
    float *data_zp_R = (float*)calloc((zp_N),sizeof(float));                //zero padding 右聲道
    short *lpf_L = (short*)calloc((zp_N+P-1),sizeof(short));                //zero padding through lpf 左聲道
    short *lpf_R = (short*)calloc((zp_N+P-1),sizeof(short));                //zero padding through lpf 右聲道
    int n,i;

    /*Create LPF coefficient*/
    float *h = (float*)malloc(sizeof(float)*P);                             //LPF
    for(n=0;n<P;n++) {
        h[n] = L*low_pass(MOrder, n);
    }

    /*Make output header*/
    Wavout_head = Wavin_head;
    Wavout_head.samplehz = FS_out;
	Wavout_head.bytepsec = FS_out*Wavout_head.channels*(Wavout_head.sample_bits/8);
	Wavout_head.DATALen = Wavout_head.bytepsec*wav_time;
	Wavout_head.total_Len = 36+Wavout_head.DATALen;
    fwrite(&Wavout_head,sizeof(Wavout_head),1,fp_out);              //Write output wave header


    short *out_L = (short*)calloc(L,sizeof(short));                 //output 左聲道
    short *out_R = (short*)calloc(L,sizeof(short));                 //output 右聲道

    short prev_L[P-1] = {0};        //overlap and add
    short prev_R[P-1] = {0};        //overlap and add

while( fread(data_read, sizeof(short), data_L, fp) ) {
        /*convert data type and zero padding*/
        for(i=0;i<(data_L);i+=2) {
            data_zp_L[(i*L)/2] = (float) data_read[i];
            data_zp_R[(i*L)/2] = (float) data_read[i+1];
        }
        /*Through LPF (convolution)*/
        through_LPF(data_zp_L, zp_N , h,  lpf_L);
        through_LPF(data_zp_R, zp_N , h,  lpf_R);

        /*overlap and add*/
        overlap_add(lpf_L , out_L , prev_L);
        overlap_add(lpf_R , out_R , prev_R);


        for(i=0;i<L;i++) {
            fwrite(out_L+i, sizeof(short), 1, fp_out);
            fwrite(out_R+i, sizeof(short), 1, fp_out);
        }
    }

    free(data_read);
    free(data_zp_L);
    free(data_zp_R);
    free(lpf_L);
    free(lpf_R);
    free(h);
    free(out_L);
    free(out_R);
    fclose(fp);
    fclose(fp_out);

    return 0;
}
#include "function.h"

int main(void)
{

    FILE *fp = fopen("input.wav", "rb");           // 讀入音檔
    FILE *fp_out = fopen("output.wav", "wb");      // 開啟輸出檔案
    struct WaveHeader Wavin_head;                  // 儲存讀入wave之標頭
    struct WaveHeader Wavout_head;                 // 儲存輸出wave之標頭
    fread(&Wavin_head, sizeof(Wavin_head), 1, fp); // 讀入wave標頭
    int wav_time =
        Wavin_head.DATALen / Wavin_head.bytepsec; // 計算音檔長度(單位：秒)

    short *data_read =
        (short *)malloc(sizeof(short) * (data_L)); // 讀取wav雙聲道
    float *data_zp_L =
        (float *)calloc((zp_N), sizeof(float)); // zero padding 左聲道
    float *data_zp_R =
        (float *)calloc((zp_N), sizeof(float)); // zero padding 右聲道
    short *lpf_L = (short *)calloc(
        (zp_N + P - 1), sizeof(short)); // zero padding through lpf 左聲道
    short *lpf_R = (short *)calloc(
        (zp_N + P - 1), sizeof(short)); // zero padding through lpf 右聲道
    int n, i;

    /*Create LPF coefficient*/
    FILE *fptxt = fopen("h.txt", "w+"); // 開啟txt檔以寫入LPF coeficient
    float *h = (float *)malloc(sizeof(float) * P); // 儲存LPF
    /*Make LPF*/
    for (n = 0; n < P; n++) {
        h[n] = L * low_pass(MOrder, n);
        fprintf(fptxt, "%lf\n", h[n]); // write into txt
        fflush(fptxt);
    }
    fclose(fptxt);

    /*Make output header*/
    Wavout_head = Wavin_head;
    Wavout_head.samplehz = FS_out; // 寫入欲輸出之取樣率
    Wavout_head.bytepsec =
        FS_out * Wavout_head.channels *
        (Wavout_head.sample_bits / 8); // 計算每秒有多少byte的資料
    Wavout_head.DATALen = Wavout_head.bytepsec * wav_time; // 計算資料總大小
    Wavout_head.total_Len =
        36 + Wavout_head.DATALen; // 計算整個檔案大小(36+檔案大小)
    fwrite(&Wavout_head, sizeof(Wavout_head), 1,
           fp_out); // Write output wave header

    short *out_L = (short *)calloc(L, sizeof(short)); // output 左聲道
    short *out_R = (short *)calloc(L, sizeof(short)); // output 右聲道

    short prev_L[P - 1] = {0}; // overlap and add
    short prev_R[P - 1] = {0}; // overlap and add

    while (fread(data_read, sizeof(short), data_L, fp)) {
        /*convert data type and zero padding*/
        for (i = 0; i < (data_L); i += 2) {
            data_zp_L[(i * L) / 2] = (float)data_read[i];
            data_zp_R[(i * L) / 2] = (float)data_read[i + 1];
        }
        /*Through LPF (convolution)*/
        through_LPF(data_zp_L, zp_N, h, lpf_L);
        through_LPF(data_zp_R, zp_N, h, lpf_R);

        /*overlap and add*/
        overlap_add(lpf_L, out_L, prev_L);
        overlap_add(lpf_R, out_R, prev_R);

        for (i = 0; i < L; i++) {
            fwrite(out_L + i, sizeof(short), 1, fp_out);
            fwrite(out_R + i, sizeof(short), 1, fp_out);
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

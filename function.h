#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793f
#define data_L 441*2                // 20ms frame size 44100*0.01 = 441 (Stereo = 441 X 2 = 882)
#define FL 22050.0f                 // cutoff frequency = 22050 Hz
#define FS 44100.0f                 // sampling frequency
#define MOrder 512                  // LPF Order
#define M 441                        
#define L 80
#define P (2*MOrder+1)
#define zp_N M*(data_L/2)



float hamming(int N, int n);
float low_pass(int m, int n);
void through_LPF(float *data, int data_length, float *h, int h_length, float *output);










#endif
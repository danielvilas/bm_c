//
// Created by Daniel Vilas Perulan on 4/11/17.
//

#include <printf.h>
#include "process-basic.h"

#include <fftw3.h>
#include <math.h>

#define FFT_SIZE 300
#define REAL 0
#define IMAG 1

double getMagnitude(fftw_complex out[], int i){
    int pos = i*(FFT_SIZE/2)/500;

    double re = out[pos][REAL];
    double im = out[pos][IMAG];
    return sqrt(re*re+im*im);
}


void executeFft(pLogPacket logPacket, int offset, fftw_complex in[],fftw_complex out[], fftw_plan plan, double outData[]){
    double average=0.0;
    for(int i=0;i<FFT_SIZE;i++){
        double value = (logPacket->logData[i+offset].A0 -512.0)/512.0;
        in[i][REAL]=value;
        in[i][IMAG]=0;
        average+=fabs(value);
    }
    average /= FFT_SIZE;
    fftw_execute(plan);
    outData[0] = getMagnitude(out,50);
    outData[1] = getMagnitude(out,150);
    outData[2] = getMagnitude(out,250);
    outData[3] = getMagnitude(out,350);
    outData[4] = average;

}

void basicProcess(pLogPacket logPacket, pCallbacks callbacks){
    fftw_complex in[FFT_SIZE], out[FFT_SIZE];
    fftw_plan plan;
    plan = fftw_plan_dft_1d(FFT_SIZE,in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    double data0[5],data3[5], data6[5], data9[5];

    executeFft(logPacket,0, in, out, plan, data0);
    executeFft(logPacket,300, in, out, plan, data3);
    executeFft(logPacket,600, in, out, plan, data6);
    executeFft(logPacket,1023-300, in, out, plan, data9);

    fftw_destroy_plan(plan);

}
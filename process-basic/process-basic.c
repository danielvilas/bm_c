//
// Created by Daniel Vilas Perulan on 4/11/17.
//

#include <printf.h>
#include "process-basic.h"

#include <fftw3.h>
#include <math.h>
#include <doublefann.h>

#define FFT_SIZE 300
#define REAL 0
#define IMAG 1

double getMagnitude(fftw_complex out[], int i){
    int pos = i*(FFT_SIZE/2)/500;

    double re = out[pos][REAL];
    double im = out[pos][IMAG];
    return sqrt(re*re+im*im);
}


void executeFft(pLogPacket logPacket, int offset, fftw_complex in[],fftw_complex out[], fftw_plan plan, fann_type outData[]){
    double average=0.0;
    for(int i=0;i<FFT_SIZE;i++){
        double value = (logPacket->logData[i+offset].A0 -512.0)/512.0;
        in[i][REAL]=value;
        in[i][IMAG]=0;
        average+=fabs(value);
    }
    average /= FFT_SIZE;
    fftw_execute(plan);
    outData[0] = (fann_type) getMagnitude(out,50);
    outData[1] = (fann_type) getMagnitude(out,150);
    outData[2] = (fann_type) getMagnitude(out,250);
    outData[3] = (fann_type) getMagnitude(out,350);
    outData[4] = (fann_type) average;

}

void fillData(fann_type *data, fann_type* res){
    for(int i=0;i<4;i++){
        data[i]+=res[i];
    }
}

void basicProcess(pLogPacket logPacket, pCallbacks callbacks){
    fftw_complex in[FFT_SIZE], out[FFT_SIZE];
    fftw_plan plan;
    plan = fftw_plan_dft_1d(FFT_SIZE,in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fann_type data0[5],data3[5], data6[5], data9[5];

    executeFft(logPacket,0, in, out, plan, data0);
    executeFft(logPacket,300, in, out, plan, data3);
    executeFft(logPacket,600, in, out, plan, data6);
    executeFft(logPacket,1023-300, in, out, plan, data9);

    fftw_destroy_plan(plan);

    fann_type data[4], res0[4], res3[4], res6[4], res9[4];
    for(int i=0;i<4;i++) {
        data[i] = 0.0;
        res0[i] = 0;
        res3[i] = 0;
        res6[i] = 0;
        res9[i] = 0;
    }
    fann_type *res;
    res=fann_run(callbacks->net, data0);
    //fann_descale_output(callbacks->net, res);
    fillData(data, res);

    res=fann_run(callbacks->net, data3);
    //fann_descale_output(callbacks->net, res);
    fillData(data, res);

    res=fann_run(callbacks->net, data6);
    //fann_descale_output(callbacks->net, res);
    fillData(data, res);

    res=fann_run(callbacks->net, data9);
   //fann_descale_output(callbacks->net, res);
    fillData(data, res);

    for(int i=0;i<4;i++){
        data[i]/=4;
    }

    tParsedPacket packet;
    readParsedPacket(&packet,logPacket->logData[0].date,data);
    callbacks->client->send(callbacks->client,&packet);


}
//
// Created by Daniel Vilas Perulan on 4/11/17.
//

#include <stdio.h>
#include "types.h"
#include <inttypes.h>

void readLogData(pLogData data, char* str){
    //printf("%s",str);
    uint64_t micros;
    uint16_t a0;
    uint16_t a1;
    uint64_t date;
    uint16_t delta;

    sscanf(str,"%"SCNx64", %"SCNu16", %"SCNu16", %"SCNu64", %"SCNu16,&micros,&a0,&a1,&date,&delta);
    //"SCNx64"
    //printf("%u, %u, %u, %u, %u\n",micros,a0,a1,date,delta);

    data->A0=a0;
    data->A1=a1;
    data->date=date;
    data->deltaMicros=delta;
    data->micros=micros;
}
//"b682db0, 508, 521, 1500212162190, 191376816\n"


void readParsedPacket(pParsedPacket packet, uint64_t date, double* result){
    packet->date=date;
    packet->tv=result[0];
    packet->bluray=result[1];
    packet->appleTv=result[2];
    packet->ipTv=result[3];
}
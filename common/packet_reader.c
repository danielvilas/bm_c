//
// Created by Daniel Vilas Perulan on 4/11/17.
//

#include "packet_reader.h"
#include <stdio.h>

tLogPacket packet;
int pos=0;

void packageReader(pLogData logData, pCallbacks callbacks){
    packet.logData[pos++]=*logData;
    if(pos==LOG_PACKET_SIZE){
        pos=0;
        callbacks->pushLogPacket(&packet,callbacks);
    }
}
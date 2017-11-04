//
// Created by Daniel Vilas Perulan on 4/11/17.
//

#ifndef BMC_TYPES_H
#define BMC_TYPES_H


#include <stdint.h>

typedef struct sLogData tLogData;
typedef tLogData* pLogData;

struct sLogData{
    uint16_t A0;
    uint16_t A1;
    uint64_t date;
    uint16_t deltaMicros;
    uint64_t micros;
};

#define LOG_PACKET_SIZE 1024

typedef struct sLogPacket tLogPacket;
typedef tLogPacket* pLogPacket;

struct sLogPacket{
    tLogData logData[LOG_PACKET_SIZE];
};



void readLogData(pLogData data, char* str);


#endif //BMC_TYPES_H

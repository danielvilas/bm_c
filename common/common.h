#ifndef COMMON_H
#define COMMON_H

#include "cmdparse.h"
#include "client.h"
#include "types.h"
#include <stdint.h>
#include <doublefann.h>

void printCommon(void);

void app_main(pCfg cfg, pClient client);

uint64_t get_posix_clock_time (void);

typedef struct sCallbacks tCallbacks;
typedef tCallbacks* pCallbacks;

struct sCallbacks{
    pClient client;
    struct fann *net;
    void (*pushLogData)(pLogData logData, pCallbacks callbacks);
    void (*pushLogPacket)(pLogPacket logPacket, pCallbacks callbacks);
};

#endif

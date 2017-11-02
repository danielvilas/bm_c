#ifndef COMMON_H
#define COMMON_H

#include "cmdparse.h"
#include "client.h"
#include <stdint.h>

void printCommon(void);

void app_main(pCfg cfg, pClient client);

uint64_t get_posix_clock_time (void);

typedef struct sCallbacks tCallbacks;
typedef tCallbacks* pCallbacks;

struct sCallbacks{
    pClient client;
    int (*init)(pClient self, pCfg cfg);
    int (*send)(pClient self, char *data, unsigned int len );
};

#endif

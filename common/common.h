#ifndef COMMON_H
#define COMMON_H

#include "cmdparse.h"
#include "client.h"
#include <stdint.h>

void printCommon(void);

void app_main(pCfg cfg, pClient client);

uint64_t get_posix_clock_time (void);

#endif

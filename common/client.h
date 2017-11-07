#ifndef CLIENT_H
#define CLIENT_H
#include "cmdparse.h"
#include "types.h"

typedef struct sClient tClient;
typedef tClient* pClient;
struct sClient{
    void *obj;
    int (*init)(pClient self, pCfg cfg);
    int (*send)(pClient self, pParsedPacket data);
    int (*close)(pClient self);
};



#endif

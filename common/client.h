#ifndef CLIENT_H
#define CLIENT_H
#include "cmdparse.h"

typedef struct sClient tClient;
typedef tClient* pClient;
struct sClient{
    void *obj;
    int (*init)(pClient self, pCfg cfg);
    int (*send)(pClient self, char *data, unsigned int len );
};



#endif

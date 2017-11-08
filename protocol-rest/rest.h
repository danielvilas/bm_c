//
// Created by dvilaspe on 08/11/2017.
//

#ifndef BMC_REST_H
#define BMC_REST_H

#include <curl/curl.h>
#include "../common/client.h"

typedef struct sRestObjs tRestObjs;
typedef tRestObjs* pRestObjs;
struct sRestObjs{
    char* url;
    CURL  *hnd;
};

pClient createRestClient(void);
#endif //BMC_REST_H

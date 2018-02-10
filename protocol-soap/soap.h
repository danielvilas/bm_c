//
// Created by dvilaspe on 08/11/2017.
//

#ifndef BMC_SOAP_H
#define BMC_SOAP_H

#include <curl/curl.h>
#include "../common/client.h"

typedef struct sSoapObjs tSoapObjs;
typedef tSoapObjs* pSoapObjs;
struct sSoapObjs{
    char* url;
    CURL  *hnd;
};

pClient createSoapClient(void);
#endif //BMC_SOAP_H

//
// Created by dvilaspe on 08/11/2017.
//

#include <stdlib.h>
#include "rest.h"
#include <curl/curl.h>
#include <inttypes.h>
#include <memory.h>

int rest_init(pClient self, pCfg cfg);
int rest_send(pClient self, pParsedPacket data );
int rest_close(pClient self);

int sentMessagesRest=0;

pClient createRestClient(void){
    pClient ret = (pClient)malloc(sizeof(tClient));
    ret->init=rest_init;
    ret->send=rest_send;
    ret->close =rest_close;
    pRestObjs client = malloc(sizeof(tRestObjs));
    client->hnd =curl_easy_init();
    ret->obj=client;
    return ret;
}

int rest_init(pClient self, pCfg cfg){
    pRestObjs client =self->obj;
    CURL *hnd = client->hnd;

    char address[1024];
    memset(address,0,1024);
    char* tmp_pr=strstr(cfg->server,"://");
    if(tmp_pr==NULL){
        sprintf(address,"http://%s",cfg->server);
        tmp_pr=strstr(address,"://");
    }else{
        strcpy(address,cfg->server);
        tmp_pr=strstr(address,"://");
    }

    char* tmp_po = strstr(tmp_pr+1,":");
    if(tmp_po==NULL){
        sprintf(address,"%s:9090/api/addPacket",address);
    }else{
        sprintf(address,"%s/api/addPacket",address);
    }

    printf("%s\n",address);


    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, address);

    return 0;
}
int rest_send(pClient self, pParsedPacket data ){
    pRestObjs client =self->obj;
    CURL *hnd = client->hnd;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "content-type: application/json");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    char tmp[1024];
    int len = sprintf(tmp,"{"
    "\"date\": %"SCNu64","
            "\"tvSeconds\": %lf,"
            "\"bluraySeconds\": %lf,"
            "\"appleTvSeconds\": %lf,"
            "\"ipTvSeconds\":  %lf"
            "}",data->date, data->tv,data->bluray,data->appleTv,data->ipTv);

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, tmp);
    sentMessagesRest++;
    CURLcode ret = curl_easy_perform(hnd);

    return 0;
}
int rest_close(pClient self){
    pRestObjs client =self->obj;
    CURL *hnd = client->hnd;
    curl_easy_duphandle(hnd);
    printf("Messages: %i\n",sentMessagesRest);
    return 0;
}
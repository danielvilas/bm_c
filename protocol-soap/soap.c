//
// Created by dvilaspe on 08/11/2017.
//

#include <stdlib.h>
#include "soap.h"
#include <curl/curl.h>
#include <inttypes.h>
#include <memory.h>

int soap_init(pClient self, pCfg cfg);
int soap_send(pClient self, pParsedPacket data );
int soap_close(pClient self);
int sentMessagesSoap=0;

pClient createSoapClient(void){
    pClient ret = (pClient)malloc(sizeof(tClient));
    ret->init=soap_init;
    ret->send=soap_send;
    ret->close =soap_close;
    pSoapObjs client = malloc(sizeof(tSoapObjs));
    client->hnd =curl_easy_init();
    ret->obj=client;
    return ret;
}

size_t soap_write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

int soap_init(pClient self, pCfg cfg){
    pSoapObjs client =self->obj;
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
        sprintf(address,"%s:9090/ws",address);
    }else{
        sprintf(address,"%s/ws",address);
    }

    printf("%s\n",address);


    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, address);
    //disable log
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, soap_write_data);

    return 0;
}
int soap_send(pClient self, pParsedPacket data ){
    pSoapObjs client =self->obj;
    CURL *hnd = client->hnd;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "content-type: text/xml");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    char tmp[1024];
    time_t time = (long)( data->date)/1000;
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&time));

    int len = sprintf(tmp, "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:data=\"http://daniel.es/outputgui/data\">\n"
            "   <soapenv:Header/>\n"
            "   <soapenv:Body>\n"
            "      <data:AddPacketRequest>\n"
            "         <data:packet>\n"
            "            <data:date>%s</data:date>\n"
            "            <data:tvSeconds>%lf</data:tvSeconds>\n"
            "            <data:bluraySeconds>%lf</data:bluraySeconds>\n"
            "            <data:appleTvSeconds>%lf</data:appleTvSeconds>\n"
            "            <data:ipTvSeconds>%lf</data:ipTvSeconds>\n"
            "         </data:packet>\n"
            "      </data:AddPacketRequest>\n"
            "   </soapenv:Body>\n"
            "</soapenv:Envelope>", buf, data->tv, data->bluray, data->appleTv, data->ipTv);

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, tmp);
    sentMessagesSoap++;
    CURLcode ret = curl_easy_perform(hnd);

    return 0;
}
int soap_close(pClient self){
    pSoapObjs client =self->obj;
    CURL *hnd = client->hnd;
    curl_easy_duphandle(hnd);
    printf("Messages: %i\n",sentMessagesSoap);
    return 0;
}

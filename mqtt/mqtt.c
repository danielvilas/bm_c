#include "mqtt.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include <MQTTClient.h>

#define CLIENTID    "bmc_mqtt"
#define TOPIC       "bmmqtt/data"
#define QOS         1
#define TIMEOUT     10000L

int mqtt_init(pClient self, pCfg cfg);
int mqtt_send(pClient self, char *data, unsigned int len );

pClient createMqttClient(void){
    pClient ret = (pClient)malloc(sizeof(tClient));
    ret->init=mqtt_init;
    ret->send=mqtt_send;
    MQTTClient *client;
    client =( MQTTClient*) malloc(sizeof(MQTTClient));
    ret->obj=client;
    return ret;
}

int mqtt_init(pClient self, pCfg cfg){

    MQTTClient *client =(MQTTClient *)self->obj;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
        conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    printf("Request connection to %s -> ",cfg->server);
    int rc;
    char address[255];
    memset(address,0,255);
    char* tmp_pr=strstr(cfg->server,"://");
    if(tmp_pr==NULL){
        sprintf(address,"tcp://%s",cfg->server);
        tmp_pr=strstr(address,"://");
    }else{
        strcpy(address,cfg->server);
        tmp_pr=strstr(address,"://");
    }

    char* tmp_po = strstr(tmp_pr+1,":");
    if(tmp_po==NULL){
        sprintf(address,"%s:1883",address);
    }
    printf("%s\n",address);
    MQTTClient_create(client, address, CLIENTID,
    MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if ((rc = MQTTClient_connect(*client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    return 0;
}
int mqtt_send(pClient self, char *data, unsigned int len ){
    //printf("mqtt_send(%x):%s [%i]\n",self,data, strlen(data));
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = data;
    pubmsg.payloadlen = strlen(data);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient *client =(MQTTClient *)self->obj;
    //int rc;
    //shot and forgot
    MQTTClient_publishMessage(*client, TOPIC, &pubmsg, &token);
    //printf("Waiting for up to %d seconds for publication of %s\n"
    //        "on topic %s for client with ClientID: %s\n",
    //        (int)(TIMEOUT/1000), data, TOPIC, CLIENTID);
    //rc = MQTTClient_waitForCompletion(*client, token, TIMEOUT);
    //printf("Message with delivery token %d delivered\n", token);

     return 0;
}

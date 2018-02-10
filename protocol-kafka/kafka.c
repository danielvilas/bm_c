//
// Created by dvilaspe on 07/11/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <jansson.h>
#include <errno.h>

#include "kafka.h"

#define BROKER_PATH "/brokers/ids"
const int debug=0;

static rd_kafka_t *rk;

int kafka_init(pClient self, pCfg cfg);
int kafka_send(pClient self, pParsedPacket data );
int kafka_close(pClient self);


pClient createKafkaClient(void){
    pClient ret = (pClient)malloc(sizeof(tClient));
    ret->init=kafka_init;
    ret->send=kafka_send;
    ret->close=kafka_close;
    pKafkaObjs *client;
    client =( pKafkaObjs*) malloc(sizeof(pKafkaObjs));
    ret->obj=client;
    return ret;
}

static void set_brokerlist_from_zookeeper(zhandle_t *zzh, char *brokers)
{
    if (zzh)
    {
        struct String_vector brokerlist;
        if (zoo_get_children(zzh, BROKER_PATH, 1, &brokerlist) != ZOK)
        {
            fprintf(stderr, "No brokers found on path %s\n", BROKER_PATH);
            return;
        }

        int i;
        char *brokerptr = brokers;
        for (i = 0; i < brokerlist.count; i++)
        {
            char path[255], cfg[1024];
            sprintf(path, "/brokers/ids/%s", brokerlist.data[i]);
            int len = sizeof(cfg);
            zoo_get(zzh, path, 0, cfg, &len, NULL);

            if (len > 0)
            {
                cfg[len] = '\0';
                json_error_t jerror;
                json_t *jobj = json_loads(cfg, 0, &jerror);
                if (jobj)
                {
                    json_t *jhost = json_object_get(jobj, "host");
                    json_t *jport = json_object_get(jobj, "port");

                    if (jhost && jport)
                    {
                        const char *host = json_string_value(jhost);
                        const int   port = json_integer_value(jport);
                        sprintf(brokerptr, "%s:%d", host, port);

                        brokerptr += strlen(brokerptr);
                        if (i < brokerlist.count - 1)
                        {
                            *brokerptr++ = ',';
                        }
                    }
                    json_decref(jobj);
                }
            }
        }
        deallocate_String_vector(&brokerlist);
        printf("Found brokers %s\n", brokers);
    }
}


static void watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    char brokers[1024];
    if (type == ZOO_CHILD_EVENT && strncmp(path, BROKER_PATH, sizeof(BROKER_PATH) - 1) == 0)
    {
        brokers[0] = '\0';
        set_brokerlist_from_zookeeper(zh, brokers);
        if (brokers[0] != '\0' && rk != NULL)
        {
            rd_kafka_brokers_add(rk, brokers);
            rd_kafka_poll(rk, 10);
        }
    }
}


static zhandle_t* initialize_zookeeper(const char * zookeeper, const int debug)
{
    zhandle_t *zh;
    if (debug)
    {
        zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
    }
    zh = zookeeper_init(zookeeper, watcher, 10000, 0, 0, 0);
    if (zh == NULL)
    {
        fprintf(stderr, "Zookeeper connection not established.");
        exit(1);
    }
    return zh;
}

/**
 * Kafka logger callback (optional)
 */
static void logger (const rd_kafka_t *rk, int level,
                    const char *fac, const char *buf) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    fprintf(stderr, "%u.%03u RDKAFKA-%i-%s: %s: %s\n",
            (int)tv.tv_sec, (int)(tv.tv_usec / 1000),
            level, fac, rd_kafka_name(rk), buf);
}


/**
 * Message delivery report callback.
 * Called once for each message.
 * See rdkafka.h for more information.
 */
static void msg_delivered (rd_kafka_t *rk,
                           void *payload, size_t len,
                           int error_code,
                           void *opaque, void *msg_opaque) {

    if (error_code)
        fprintf(stderr, "%% Message delivery failed: %s\n",
                rd_kafka_err2str(error_code));
    else
        fprintf(stderr, "%% Message delivered (%zd bytes)\n", len);
}

int kafka_init(pClient self, pCfg cfg){
    char zookeeper[255]; //Copy code from MQTT
    char brokers[1024];
    char errstr[512];
    memset(brokers,0, sizeof(brokers));
    rd_kafka_topic_conf_t *topic_conf;
    char* topic="AppliancesBucket";

    pKafkaObjs kafka= self->obj;

    /* Kafka configuration */
    kafka->conf = rd_kafka_conf_new();

    /* Topic configuration */
    topic_conf = rd_kafka_topic_conf_new();


    /* Set logger */
    rd_kafka_conf_set_log_cb(kafka->conf, logger);

    printf("Request connection to %s -> ",cfg->server);
    sprintf(zookeeper,"%s",cfg->server);
    char* tmp_po = strstr(zookeeper,":");
    if(tmp_po==NULL){
        sprintf(zookeeper,"%s:2181",zookeeper);
    }
    printf("%s\n",zookeeper);

    /** Initialize zookeeper */
    kafka->zh = initialize_zookeeper(zookeeper, debug );

    /* Add brokers */
    set_brokerlist_from_zookeeper(kafka->zh, brokers);
    if (rd_kafka_conf_set(kafka->conf, "metadata.broker.list",
                          brokers, errstr, sizeof(errstr) !=
                                           RD_KAFKA_CONF_OK)) {
        fprintf(stderr, "%% Failed to set brokers: %s\n", errstr);
        exit(1);
    }

    if (debug) {
        printf("Broker list from zookeeper cluster %s: %s\n", zookeeper, brokers);
    }



    /* Set up a message delivery report callback.
     * It will be called once for each message, either on successful
     * delivery to broker, or upon failure to deliver to broker. */
    rd_kafka_conf_set_dr_cb(kafka->conf, msg_delivered);

    /* Create Kafka handle */
    if (!(rk = rd_kafka_new(RD_KAFKA_PRODUCER, kafka->conf, errstr, sizeof(errstr)))) {
        fprintf(stderr,
             "%% Failed to create new producer: %s\n",
                errstr);
        exit(1);
    }

    /* Create topic */
    kafka->rkt = rd_kafka_topic_new(rk, topic, topic_conf);

    return 0;
}
int kafka_send(pClient self, pParsedPacket data ){
    pKafkaObjs kafka = self->obj;
    char tmp[1024];
    int partition = RD_KAFKA_PARTITION_UA;
    int len = sprintf(tmp,"{"
            "\"date\": %"SCNu64","
            "\"tv\": %lf,"
            "\"bluray\": %lf,"
            "\"appleTv\": %lf,"
            "\"ipTv\":  %lf"
            "}",data->date, data->tv,data->bluray,data->bluray,data->ipTv);

    if (rd_kafka_produce(kafka->rkt, partition,
                         RD_KAFKA_MSG_F_COPY,
            // Payload and length
                         tmp, len,
            // Optional key and its length
                         NULL, 0,
            //* Message opaque, provided in delivery report callback as msg_opaque.
            NULL) == -1) {
        fprintf(stderr,
                "%% Failed to produce to topic %s "
                        "partition %i: %i\n",
                rd_kafka_topic_name(kafka->rkt), partition, errno);
        // Poll to handle delivery reports
        rd_kafka_poll(rk, 0);
    }


    if (debug){
        fprintf(stderr, "%% Sent %zd bytes to topic "
                        "%s partition %i\n",
                len, rd_kafka_topic_name(kafka->rkt), partition);

        // Poll to handle delivery reports
        rd_kafka_poll(rk, 0);

        // Wait for messages to be delivered
        while (rd_kafka_outq_len(rk) > 0)
            rd_kafka_poll(rk, 100);
    }

    return 0;
}

int kafka_close(pClient self){
    pKafkaObjs kafka = self->obj;
    rd_kafka_destroy(rk);
    /* Let background threads clean up and terminate cleanly. */
    rd_kafka_wait_destroyed(2000);

    /** Free the zookeeper data. */
    zookeeper_close(kafka->zh);
}

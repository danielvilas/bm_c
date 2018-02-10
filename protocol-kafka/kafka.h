//
// Created by dvilaspe on 07/11/2017.
//

#ifndef BMC_KAFKA_H
#define BMC_KAFKA_H
#include "../common/client.h"
#include <librdkafka/rdkafka.h>
#include <zookeeper/zookeeper.h>

pClient createKafkaClient(void);

typedef struct sKafkaObjs tKafkaObjs;
typedef tKafkaObjs* pKafkaObjs;
struct sKafkaObjs{
    rd_kafka_conf_t *conf;
    zhandle_t *zh;
    rd_kafka_topic_t *rkt;
};


#endif //BMC_KAFKA_H

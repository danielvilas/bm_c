#include <stdio.h>
#include "common/common.h"
#include "common/cmdparse.h"
#include "mqtt/mqtt.h"
#include "protocol-kafka/kafka.h"


int main(int argc, char** argv){
	tCfg cfg;
	printf("Hello World\n");
	printCommon();
	if(!parseCfg(argc, argv, &cfg)){
		pClient client;
		switch(cfg.proto){
		case MQTT:
			client = createMqttClient();break;
		case WS:
			printf("WS\n");break;
		case KAFKA:
			client=createKafkaClient();break;
		case UNDEFINED:
			printf("UNDEFINED\n");break;
		}
		app_main(&cfg,client);
	}
}

#include <stdio.h>
#include "common/common.h"
#include "common/cmdparse.h"
#include "mqtt/mqtt.h"
#include "protocol-kafka/kafka.h"
#include "protocol-rest/rest.h"
#include "protocol-soap/soap.h"


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
			client = createSoapClient();break;
		case KAFKA:
			client=createKafkaClient();break;
        case REST:
	        client=createRestClient();break;
		case UNDEFINED:
			printf("UNDEFINED\n");return -1;
		}
		app_main(&cfg,client);
	}
return (0);
}

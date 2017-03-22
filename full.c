#include <stdio.h>
#include "common/common.h"
#include "common/cmdparse.h"
#include "mqtt/mqtt.h"

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
			printf("KAFKA\n");break;
		case UNDEFINED:
			printf("UNDEFINED\n");break;
		}
		app_main(&cfg,client);
	}
}

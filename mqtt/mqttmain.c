#include <stdio.h>
#include "../common/common.h"
#include "../common/cmdparse.h"
#include "mqtt.h"

int main(int argc, char** argv){
	tCfg cfg;
	printf("C BenchMark MQTT\n");
	printCommon();
	if (!parseCfg(argc, argv, &cfg)){
		pClient client = createMqttClient();
		app_main(&cfg,client);
	}
}

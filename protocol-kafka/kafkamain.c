//
// Created by dvilaspe on 07/11/2017.
//

#include <stdio.h>
#include "../common/cmdparse.h"
#include "../common/common.h"
#include "kafka.h"

int main(int argc, char** argv){
    tCfg cfg;
    printf("C BenchMark KAFKA\n");
    printCommon();
    if (!parseCfg(argc, argv, &cfg)){
        pClient client = createKafkaClient();
        app_main(&cfg,client);
    }
}
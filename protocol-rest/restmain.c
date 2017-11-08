//
// Created by dvilaspe on 08/11/2017.
//

#include "rest.h"
#include "../common/common.h"

int main(int argc, char** argv){
    tCfg cfg;
    printf("C BenchMark REST\n");
    printCommon();
    if (!parseCfg(argc, argv, &cfg)){
        pClient client = createRestClient();
        app_main(&cfg,client);
    }
}

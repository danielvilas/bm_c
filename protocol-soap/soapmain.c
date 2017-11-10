//
// Created by dvilaspe on 08/11/2017.
//

#include "soap.h"
#include "../common/common.h"

int main(int argc, char** argv){
    tCfg cfg;
    printf("C BenchMark SOAP\n");
    printCommon();
    if (!parseCfg(argc, argv, &cfg)){
        pClient client = createSoapClient();
        app_main(&cfg,client);
    }
}

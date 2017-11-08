#ifndef CMD_PARSE_H
#define CMD_PARSE_H
#include <stdbool.h> 

#ifdef FULL
enum eProtocol{UNDEFINED,MQTT,WS,KAFKA,REST};
typedef enum eProtocol tProtocol;

#endif

enum eParseResult{ PARSE_OK = 0, PARSE_ERROR=-1, PARSE_PRINTHELP};
typedef enum eParseResult tParseResult;

struct sCfg{
    char* server;
    bool continous;
    char* dataset;
#ifdef FULL
    tProtocol proto; 
#endif
};

typedef struct sCfg tCfg;
typedef tCfg* pCfg;

tParseResult parseCfg(int argc, char** argv, pCfg cfg);
void printHelp(char *name);
void printConfig(pCfg cfg);
#endif

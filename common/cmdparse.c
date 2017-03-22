#include <stdio.h>
#include "cmdparse.h"
#include <getopt.h>
#include <string.h>

#ifdef FULL
  static char* short_options="s:c:d:p:h";
#else
  static char* short_options="s:c:d:h";
#endif

static struct option long_options[] =
{
    {"server", required_argument, NULL, 's'},
	{"continous", required_argument, NULL, 'c'},
	{"dataset", required_argument, NULL, 'd'},
	{"help", no_argument, NULL, 'h'},
#ifdef FULL
    {"protocol", required_argument, NULL, 'p'},
#endif
    {NULL, 0, NULL, 0}
};

tParseResult parseCfg(int argc, char** argv, pCfg cfg){
	if (argc==1){
		printHelp(argv[0]);
		return PARSE_PRINTHELP;
	}
#ifdef FULL
	printf("Parsing full\n");
	cfg->proto=UNDEFINED;
#else
	printf("Parsing cmd\n");
#endif

cfg->continous=false;
cfg->dataset=NULL;
cfg->server="server.local";


int i;
int c = getopt_long(argc, argv, short_options, long_options, &i);
do{
	//printf("option %i(%c)[%i] -> %s\n", c,c,i,optarg);
	switch(c){
		case 'h':
			printHelp(argv[0]);
			return PARSE_PRINTHELP;
		case '?':
			printHelp(argv[0]);
			return PARSE_ERROR;
		case 's':
			cfg->server=optarg;
			break;
		case 'c':
			cfg->continous=true;
			break;
		case 'd':
			cfg->dataset=optarg;
			break;
	#ifdef FULL
		case 'p':
			if(!strcasecmp(optarg,"MQTT")){
				cfg->proto=MQTT;
			}else if (!strcasecmp(optarg,"WS")){
				cfg->proto=WS;
			}else if (!strcasecmp(optarg,"KAFKA")){
				cfg->proto=KAFKA;
			}else{
				printHelp(argv[0]);
				return PARSE_ERROR;
			} 
			break;
	#endif
	}
	c = getopt_long(argc, argv, short_options, long_options, &i);
}while(c!=-1);

if(cfg->dataset==NULL){
	printf("Need a DataFile\n");
	printHelp(argv[0]);
	return PARSE_ERROR;
}
#ifdef FULL
if(cfg->proto==UNDEFINED){
	printf("Need a Protocol\n");
	printHelp(argv[0]);
	return PARSE_ERROR;
}
#endif
printConfig(cfg);
return PARSE_OK;
}

void printHelp(char *name){
	printf("USAGE: %s OPTION1] [OPTION2]...\nThe following options are supported:\n",name);
	printf(" -s, --server <server>\tServer IP to use (\"server.local\" by default)\n");
	printf(" -d, --dataset <datafile>\tDataSet to use\n");
	printf(" -c, --continous\tEnable continuos mode\n");
#ifdef FULL
    printf(" -p, --protocol\tClient to use MQTT|WS|Kafka\n");
#endif
}

void printConfig(pCfg cfg){
#ifdef FULL
printf("Protocol: ");
switch(cfg->proto){
	case MQTT:
		printf("MQTT\n");break;
	case WS:
		printf("WS\n");break;
	case KAFKA:
		printf("KAFKA\n");break;
	case UNDEFINED:
		printf("UNDEFINED\n");break;
}
#endif

printf("DataSet: %s\n",cfg->dataset);
printf("Server: %s\n",cfg->server);
printf("Mode: ");
if(cfg->continous){
	printf("Continuos\n");
}else{
	printf("Test\n");
}


}

#include <stdio.h>
#include "common.h"
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <unistd.h>


uint16_t arduinoAdc(int sample);

void printCommon(void){
	printf("Start "VERSION"\n");
}

/*
Simulates an 1V 50hz in 2.5 bias @1Mhz fs
Output is 5V == 1023 
*/
#define PI 3.14159265
uint16_t arduinoAdc(int sample){
    float time = sample/1000.0;
    float rad = 2*PI*50*time;
    float res = 2.5 + sin(rad);
    return 1023/res;
}

void app_main(pCfg cfg, pClient client){
	client->init(client, cfg);
	uint64_t last_event = get_posix_clock_time();
    uint64_t next_event = last_event + 1000;
    int i=0;
	while(true){
		uint16_t read=arduinoAdc(i++);
        char tmp[255];
		uint8_t msg_len = sprintf(tmp,"{time:%llu, value:%u}",get_posix_clock_time()/1000, read);
        client->send(client,tmp,msg_len);
        uint64_t now= get_posix_clock_time();
        last_event = next_event;
        next_event = last_event + 1000;
        if(now>last_event){
            int t=0;
            while(now>last_event){
                last_event+=1000;
                t++;
            }
            printf("lost (%i)events\n",t);
            next_event = last_event + 1000;
        }
        usleep(last_event-now);

    }
}


uint64_t get_posix_clock_time (void){
 struct timespec ts;
 #ifndef __MACH__
#ifdef _SC_MONOTONIC_CLOCK
if (sysconf (_SC_MONOTONIC_CLOCK) > 0) {
    /* A monotonic clock presents */
	    if (clock_gettime (CLOCK_MONOTONIC, &ts) == 0)
        return (uint64_t) (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
    else
        return 0;
}
#endif
#endif
    struct timeval tv;

    if (gettimeofday (&tv, NULL) == 0)
        return (uint64_t) (tv.tv_sec * 1000000 + tv.tv_usec);
    else
        return 0;

}

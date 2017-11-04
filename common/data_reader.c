//
// Created by Daniel Vilas Perulan on 2/11/17.
//

#include "data_reader.h"
#include "types.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

tLogData last;

void readFileContents(struct dirent *dir, char* path, pCallbacks callbacks){
    printf("%s\n", dir->d_name);
    char pathname[1024];

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    tLogData log;

    sprintf( pathname, "%s/%s", path, dir->d_name );

    fp = fopen(pathname, "r");
    if (fp == NULL){
        printf("Not Found: %s (%s)\n",pathname,strerror(errno));
        return;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
        if(line[0]=='#')continue;
        readLogData(&log,line);
        if(last.micros==0){
            last=log;
        }else{
            if(last.micros>log.micros && log.micros!=-1){
                fprintf(stderr,"Dataset not clear (micros), parse in java spliter first");
            }
            if(last.date>log.date && log.micros!=-1){
                fprintf(stderr,"Dataset not clear (date), parse in java spliter first");
            }
        }

        callbacks->pushLogData(&log,callbacks);
    }

    fclose(fp);
    if (line)
        free(line);
}

void readData(pCfg cfg, pCallbacks callbacks){
    last.micros=0;
    printf("Reading Dataset: %s\n",cfg->dataset);

    char path[256];
    sprintf(path,"./data/sets/%s",cfg->dataset);

    DIR           *d;
    struct dirent *dir;
    d = opendir(path);
    int i=0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (i>0 && dir->d_type == DT_REG) //i>0 omite el de las 15 que el primer paquete esta mal
            {
                readFileContents(dir,path, callbacks);
            }
            i++;
        }

        closedir(d);
    }else{
        printf("Not Found: %s (%s)\n",path,strerror(errno));
    }

}
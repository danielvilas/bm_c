//
// Created by Daniel Vilas Perulan on 2/11/17.
//

#include "data_reader.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <memory.h>

void readFile(struct dirent *dir){
    printf("%s\n", dir->d_name);
}

void readData(pCfg cfg, pCallbacks callbacks){
    printf("Reading Dataset: %s\n",cfg->dataset);

    char path[256];
    sprintf(path,"./data/sets/%s\0",cfg->dataset);

    DIR           *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                readdir(d);
            }
        }

        closedir(d);
    }else{
        printf("Not Found: %s (%s)\n",path,strerror(errno));
    }

}
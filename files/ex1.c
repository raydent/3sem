
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <dirent.h>
#include <sys/stat.h>
int main(){
    DIR* curdir = opendir(".");
    int count = 0;
    struct dirent *temp = readdir(curdir);
    struct stat buf;
    char* name = (char*) calloc (100, sizeof(char));
    while(temp){
        strcpy(name, "./");
        strcat(name, temp -> d_name);
        count++;
        stat(name, &buf);
        int type = 0;
        //if (S_ISREG(buf . st_mode))
        printf("type = %d, size = %ld, name = %s\n", S_ISREG(buf . st_mode), buf . st_size, temp -> d_name);
        temp = readdir(curdir);
    }
    closedir(curdir);
    free(name);
    printf("count = %d", count);
}

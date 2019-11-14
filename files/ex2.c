
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

char* searching(int* n, char* curpath, char* search_name);

int main(int argc, char *argv[]){ // первым аргументом подаем название файла, который ищем, вторым - глубину поиска
    if (argc != 3){
        printf("wrong number of command line args, need %d more of them\n", 3 - argc);
        exit(0);
    }
    DIR* curdir = opendir("..");
    int count = 0;
    struct dirent *temp = readdir(curdir);
    struct stat buf;
    char* search_name = argv[1];
    int n = 0;
    for(int i = 0; i < strlen(argv[2]); i++){
        n = argv[2][i] - '0';
    }
    //int n = 2;
    char* name = searching(&n, ".", search_name);
    printf("name = %s\n", name);
}

char* searching(int* n, char* curpath, char* search_name){
    DIR* curdir = opendir(curpath);
    struct dirent *temp = readdir(curdir);
    char* name = (char*) calloc (100, sizeof(char));
    struct stat buf;
    while(temp){
        //memset(name, '-', 100);
        strcpy(name, curpath);
        strcat(name, "/");
        strcat(name, temp -> d_name);
        printf("name = %s\n", name);
        if(strcmp(temp -> d_name, ".") != 0 && strcmp(temp -> d_name, "..") != 0){
            stat(name, &buf);
            printf("type = %d, size = %ld, name = %s\n", S_ISREG(buf . st_mode), buf . st_size, temp -> d_name);
            int type = 0;
            if (S_ISREG(buf . st_mode)){
                printf("strcmp = %d, search_name = %s, temp = %s\n", strcmp(search_name , temp -> d_name), search_name, temp -> d_name);
                if(strcmp(search_name , temp -> d_name) == 0){
                    printf("type = %d, size = %ld, name = %s\n", S_ISREG(buf . st_mode), buf . st_size, temp -> d_name);
                    return name;
                }
            }
            if(S_ISDIR(buf . st_mode)){
                    if (*n > 0){
                        *n--;
                        //strcat(name, temp -> d_name);
                        char* temp1 = NULL;
                        temp1 = searching(n, name, search_name);
                        printf("temp = %s\n", temp1);
                        if (temp1)
                            return temp1;
                        *n++;
                    }
            }
        }
        temp = readdir(curdir);
    }
    free(name);
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

const int N = 100;

typedef struct mystruct {
    char* commandline;
    char** args;
    int waittime;
    int argnum;
}container_t;

int getargnum(char* string);
char** makeargarr(char* string);
container_t* fillcontainer(int stringnum , FILE* f);

int main(){
    time_t t = time(NULL);
    int stringnum = 0;
    FILE* f = fopen("source.txt", "r");
    fscanf(f, "%d", &stringnum);
    char* buf = (char*) calloc (N, sizeof(char*));
    fgets(buf, N, f);
    //stringnum = (buf[0] - 9);
    container_t* container = fillcontainer(stringnum , f);
    int frk = 1;
    for(int i = 0; i < stringnum - 1; i++){
        frk = fork();
        if (frk == 0){
            sleep(container[i].waittime);
            if (abs(time(NULL) - t) > 5){
                printf("process number %d was terminated\n", i);
                kill(getpid(), SIGKILL);
            }
            execvp(container[i].args[1], container[i].args + 1);
        }
    }
    for(int i = 0; i <= container[stringnum - 1].argnum; i++){
    }
    if (frk > 0){
        wait(NULL);
        sleep(container[stringnum - 1].waittime);
        if (abs(time(NULL) - t) > 5){
            printf("process number %d was terminated\n", stringnum);
            kill(getpid(), SIGKILL);
        }
        execvp(container[stringnum - 1].args[1], container[stringnum - 1].args + 1);
    }
    fclose(f);
}
int getargnum(char* string){
    char* space = string;
    int n = 0;
    while(space != NULL) {
        space = strstr(space + 1, " ");
        n++;
    }
    return n;
}
char** makeargarr(char* string){
    int argnum = getargnum(string);
    char** argarr = (char**) calloc (argnum + 1, sizeof(char*));
    char *p1, *p2;
    p1 = string;
    p2 = string;
    int i = 0;
    while(p2 != NULL){
        p2 = strstr(p2, " ");
        if (p2 == NULL && p1 != NULL){
            argarr[i] = (char*) calloc (strlen(string) - (p1 - string), sizeof(char));
            strcpy(argarr[i], p1);
            break;
        }
        argarr[i] = (char*) calloc (p2 - p1, sizeof(char));
        strncpy(argarr[i], p1, p2 - p1);
        p2++;
        if(p2 == NULL)
            break;
        p1 = p2;
        i++;
    }
    argarr[i + 1] = NULL;
    return argarr;
}
container_t* fillcontainer(int stringnum , FILE* f){
    container_t* container = (container_t*) calloc (stringnum, sizeof(container_t));
    for(int i = 0; i < stringnum; i++){
        container[i].commandline = (char*) calloc (N, sizeof(char));
        fgets(container[i].commandline, N, f);
        container[i].args = makeargarr(container[i].commandline);
        container[i].argnum = getargnum(container[i].commandline);
        container[i].waittime = (int)(container[i].commandline[0] - '0');
    }
    return container;
}

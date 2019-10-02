#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
const int N = 100;

typedef struct mystruct {
    char* commandline;
    char** args;
    int killtime;
    int argnum;
}container_t;

int getargnum(char* string);
char** makeargarr(char* string);
container_t* fillcontainer(int stringnum , FILE* f);

int main(){
    int stringnum = 0;
    FILE* f = fopen("source.txt", "r");
    fscanf(f, "%d", &stringnum);
    char* buf = (char*) calloc (N, sizeof(char*));
    fgets(buf, N, f);
    //stringnum = (buf[0] - 9);
    printf("stringnum = %d\n", stringnum);
    container_t* container = fillcontainer(stringnum , f);
    int frk = 1;
    printf("0\n");
    for(int i = 0; i < stringnum - 1; i++){
        printf("1\n");
        frk = fork();
        if (frk == 0){
            printf("we're here 1\n");
            //int argnum = getstringnum(stringarr[i]);

            execvp(container[i].args[1], container[i].args + 1);
            //free(argarr);
        }
    }
    printf("1\n");
    for(int i = 0; i <= container[stringnum - 1].argnum; i++){
        printf("%d string is %s\n", i, container[stringnum - 1].args[i]);
    }
    if (frk > 0){
        printf("we're here 2");
        //wait(NULL);
        //char** argarr = makeargarr(stringarr[stringnum - 1]);
        execvp(container[stringnum - 1].args[1], container[stringnum - 1].args + 1);
        //free(argarr);
    }
    fclose(f);
}
int getargnum(char* string){
    char* space = string;
    int n = 0;
    while(space != NULL) {
        //printf("%d\n", n);
        space = strstr(space + 1, " ");
        n++;
    }
    return n;
}
char** makeargarr(char* string){
    //printf("i'm dying\n");
    int argnum = getargnum(string);
    //printf("argnum = %d", argnum);
    char** argarr = (char**) calloc (argnum + 1, sizeof(char*));
    char *p1, *p2;
    p1 = string;
    p2 = string;
    int i = 0;
    while(p2 != NULL){
        p2 = strstr(p2, " ");
        if (p2 == NULL && p1 != NULL){
            printf("i = %d, p1 = %s\n", i, p1);
            argarr[i] = (char*) calloc (strlen(string) - (p1 - string), sizeof(char));
            strcpy(argarr[i], p1);
            break;
        }
        argarr[i] = (char*) calloc (p2 - p1, sizeof(char));
        strncpy(argarr[i], p1, p2 - p1);
        printf("s = %s %s", p1, p2);
        p2++;
        if(p2 == NULL)
            break;
        p1 = p2;
        i++;
    }
    argarr[i + 1] = NULL;
    //printf("now we are dying here");
    return argarr;
}
container_t* fillcontainer(int stringnum , FILE* f){
    container_t* container = (container_t*) calloc (stringnum, sizeof(container_t));
    for(int i = 0; i < stringnum; i++){
        container[i].commandline = (char*) calloc (N, sizeof(char));
        fgets(container[i].commandline, N, f);
        container[i].args = makeargarr(container[i].commandline);
        container[i].argnum = getargnum(container[i].commandline);
        container[i].killtime = (int)(container[i].commandline[0] - '0');
        //printf("string = %s", container[i].commandline);
        //printf("argnum = %d, killtime = %d\n", container[i].argnum, container[i].killtime);
    }
    return container;
}

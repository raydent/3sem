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
}container;
int getargnum(char* string);
char** makeargarr(char* string);
int main(){
    int stringnum = 0;
    FILE* f = fopen("source.txt", "r");
    fscanf(f, "%d", &stringnum);
    char* buf = (char*) calloc (N, sizeof(char*));
    fgets(buf, N, f);
    //stringnum = (buf[0] - 9);
    printf("stringnum = %d\n", stringnum);
    char** stringarr = (char**) calloc (stringnum, sizeof(char*));
    for(int i = 0; i < stringnum; i++){
        stringarr[i] = (char*) calloc (N, sizeof(char));
    }
    for(int i = 0; i < stringnum; i++){
        fgets(stringarr[i], N, f);
        printf("%s", stringarr[i]);
    }
    int frk = 1;
    printf("0\n");
    for(int i = 0; i < stringnum - 1; i++){
        printf("1\n");
        frk = fork();
        if (frk == 0){
            printf("we're here\n");
            //int argnum = getstringnum(stringarr[i]);
            char** argarr = makeargarr(stringarr[i]);
            execvp(argarr[1], argarr + 1);
            //free(argarr);
        }
    }
    printf("1\n");
    if (frk > 0){
        printf("we're here");
        //wait(NULL);
        char** argarr = makeargarr(stringarr[stringnum - 1]);
        execvp(argarr[1], argarr + 1);
        //free(argarr);
    }
    fclose(f);
}
int getargnum(char* string){
    char* space = string;
    int n = 0;
    while(space != NULL){
        space = strstr(space, " ");
    }
    return n;
}
char** makeargarr(char* string){
    printf("i'm dying\n");
    int argnum = getargnum(string);
    char** argarr = (char**) calloc (argnum + 1, sizeof(char*));
    char *p1, *p2;
    p1 = string;
    p2 = string;
    int i = 0;
    while(p2 != NULL){
        p2 = strstr(p2, " ");
        if(p2 == NULL)
            break;
        argarr[i] = (char*) calloc (p2 - p1, sizeof(char));
        strncpy(argarr[i], p1, p2 - p1);
        p1 = p2;
        i++;
    }
    argarr[i] = NULL;
    printf("now we are dying here");
    return argarr;
}

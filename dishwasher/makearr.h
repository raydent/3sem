#pragma once
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct pair{
    int num;
    char* string;
}pair;

pair* make_washer_arr(FILE* washerdata, int* size);


pair* make_washer_arr(FILE* washerdata, int* size){
    char* p = (char*) calloc (100, sizeof(char));
    int n;
    fscanf(washerdata, "%d", &n);
    fgets(p, 100, washerdata);
    pair* pairarr = (pair*) calloc (n, sizeof(pair));
    for(int i = 0; i < n; i++){
        fgets(p, 100, washerdata);
        pairarr[i].num = 0;
        for(int j = 0; j < strlen(p); ++j){
            if (p[j] >= '0' && p[j] <= '9'){
                pairarr[i].num = pairarr[i].num * 10 + p[j] - '0';
            }
        }
        char *pch = strtok (p, " ");
        int j = 0;
        pairarr[i].string = (char*) calloc (100, sizeof(char));
        strcpy(pairarr[i].string, pch);
    }
    *size = n;
    return pairarr;
}

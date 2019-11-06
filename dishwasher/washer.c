
#include "makearr.h"
const int CAPACITY = 5;
void senddish(pair pair_, FILE* table, int infosize, pair* infoarr, struct sembuf mybuf10, int semid1);
int main(){
    int semid1, semid2;
    char pathname1[] = "key1";
    key_t key1;
    struct sembuf mybuf10;
    key1 = ftok(pathname1,0);
    semid1 = semget(key1, 1, 0666 | IPC_CREAT);

    char pathname2[] = "key2";
    key_t key2;
    struct sembuf mybuf01;
    key2 = ftok(pathname2,0);
    semid2 = semget(key2, 1, 0666 | IPC_CREAT);
    FILE* source = fopen("source.dat", "r");
    FILE* washerdata = fopen("washerdata.dat", "r");
    FILE* table = fopen("table.txt", "w");
    int infosize = 0;
    int sourcesize = 0;
    pair* infoarr = make_washer_arr(washerdata, &infosize);
    pair* sourcearr = make_washer_arr(source, &sourcesize);
    printf("%s %d\n", infoarr[0].string, infoarr[0].num);
    mybuf10.sem_num = 0;
    mybuf10.sem_op = -1;
    semop(semid1, &mybuf10, 1);
    printf("goim");
    int n = CAPACITY;
    printf("n = %d", n);
    while(n > 0){
        n--;
        mybuf10.sem_num = 0;
        mybuf10.sem_op = 1;
        semop(semid1, &mybuf10, 1);
    }
    mybuf01.sem_num = 0;
    mybuf01.sem_op = 1;
    semop(semid2, &mybuf01, 1);
    for(int i = 0; i < sourcesize; i++){
        senddish(sourcearr[i], table, infosize, infoarr, mybuf10, semid1);
    }
}
void senddish(pair pair_, FILE* table, int infosize, pair* infoarr, struct sembuf mybuf10, int semid1){
    int time = 0;
    for(int i = 0; i < infosize; ++i){
        if (strcmp(pair_.string, infoarr[i].string) == 0){
            time = infoarr[i].num;
            break;
        }
    }
    for(int i = 0; i < pair_.num; ++i){
        //sleep(time);
        //fwrite(pair_.string, sizeof(char), strlen(pair_.string), table);
        fprintf(table, "%s\n", pair_.string);
        mybuf10.sem_num = 0;
        mybuf10.sem_op = -1;
        semop(semid1, &mybuf10, 1);
    }
}

#include "makearr.h"
const int CAPACITY = 5;

void getdish(FILE* table, int infosize, pair* infoarr, struct sembuf mybuf10, int semid1);
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
    FILE* washerdata = fopen("wiperdata.dat", "r");
    FILE* table = fopen("table.txt", "r");
    int infosize = 0;
    int sourcesize = 0;
    pair* infoarr = make_washer_arr(washerdata, &infosize);
    pair* sourcearr = make_washer_arr(source, &sourcesize);
    printf("%s %d\n", infoarr[0].string, infoarr[0].num);
    mybuf10.sem_num = 0;
    mybuf10.sem_op = 1;
    semop(semid1, &mybuf10, 1);
    getdish(table, infosize, infoarr, mybuf10, semid1);
}
void getdish(FILE* table, int infosize, pair* infoarr, struct sembuf mybuf10, int semid1){
    int time = 0;
    char p[100];
    while(1){
        if (fgets(p, 100, table)){
            for(int i = 0; i < infosize; ++i){
                if (strcmp(p, infoarr[i].string) == 0){
                    time = infoarr[i].num;
                    break;
                }
            }
            //sleep(time);
            mybuf10.sem_num = 0;
            mybuf10.sem_op = 1;
            semop(semid1, &mybuf10, 1);
        }
    }
}

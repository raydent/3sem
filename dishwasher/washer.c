
#include "makearr.h"
const int CAPACITY = 5;
#define LAST_MESSAGE 255
int main(){

    int   semid;
    char sempathname[]="washer.c";
    key_t semkey;
    //semctl(semid, 0, SETVAL, 0);
    struct sembuf sembuf;
    semkey = ftok(sempathname, 0);
    // semctl (semid, 0, IPC_RMID, 0);
    if((semid = semget(semkey, 1, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t create semaphore set\n");
      exit(-1);
    }
    sembuf.sem_num = 0;
    sembuf.sem_op  = -1;
    semop(semid, &sembuf, 1);

    for(int i = 0; i < CAPACITY; ++i){
        sembuf.sem_num = 0;
        sembuf.sem_op  = +1;
        semop(semid, &sembuf, 1);
    }
    // sembuf.sem_op  = -1;
    // semop(semid, &sembuf, 1);
    //semctl (semid, 0, IPC_RMID, 0);
    int infoarrsize = 0;
    int sourcearrsize = 0;
    int washerdatasize = 0;
    int wiperdatasize = 0;
    FILE* source = fopen("source.dat", "r");
    FILE* washerdata = fopen("washerdata.dat", "r");
    FILE* wiperdata = fopen("wiperdata.dat", "r");
    //FILE* table = fopen("table.txt", "w");
    pair* sourcearr =  make_washer_arr(source, &sourcearrsize);
    pair* washerdataarr =  make_washer_arr(washerdata, &washerdatasize);
    pair* wiperdataarr = make_washer_arr(wiperdata, &wiperdatasize);
    int maxlen = 100;
    int msqid;
    char pathname[]="buf";
    int key;
    int len = 50;
    int n = 0;
    struct mymsgbuf
    {
       long mtype;
       int time;
    } mybuf;
    key = ftok(pathname, 0);
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    mybuf.mtype = 1;
    printf("start\n");
    for(int i = 0; i < sourcearrsize; ++i){
        //mybuf.string =(char*) calloc (50, sizeof(char));
        //strcpy(mybuf.string, sourcearr[i].string);
        int time = 0;
        int wipertime = 0;
        for(int j = 0; j < washerdatasize; ++j){
            if (strcmp(sourcearr[i].string, washerdataarr[j].string) == 0){
                printf("found\n");
                time = washerdataarr[j].num;
                break;
            }
        }
        for(int j = 0; j < wiperdatasize; ++j){
            if (strcmp(sourcearr[i].string, wiperdataarr[j].string) == 0){
                printf("found2\n");
                printf("wipertime = %d\n", wiperdataarr[j].num);
                wipertime = wiperdataarr[j].num;
                break;
            }
        }
        mybuf.time = wipertime;
        for(int k = 0; k < sourcearr[i].num; ++k){
            sembuf.sem_op  = -1;
            semop(semid, &sembuf, 1);
            sleep(time);
            if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
            printf("sent\n");
            //sembuf.sem_num = 0;
        }
    }
    mybuf.mtype = LAST_MESSAGE;
    //msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    semctl (semid, 0, IPC_RMID, 0);
}

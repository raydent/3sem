#include "makearr.h"
const int CAPACITY = 5;
#define LAST_MESSAGE 255
int main(){

    int   semid;
    char sempathname[]="washer.c";
    key_t semkey;
    struct sembuf sembuf;
    semkey = ftok(sempathname, 0);
    //semctl (semid, 0, IPC_RMID, 0);
    if((semid = semget(semkey, 1, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t create semaphore set\n");
      exit(-1);
    }
    sembuf.sem_num = 0;
    sembuf.sem_op  = 1;
    semop(semid, &sembuf, 1);

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

    FILE* source = fopen("source.dat", "r");
    FILE* wiperdata = fopen("wiperdata.dat", "r");
    FILE* table = fopen("table.txt", "r");
    int wipersize = 0;
    pair* wiperarr = make_washer_arr(wiperdata, &wipersize);
    //msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    while(1){
        maxlen = 50;

        if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0){
          printf("Can\'t receive message from queue\n");
          exit(-1);
        }
        if (mybuf.mtype == LAST_MESSAGE) {
            printf("got lastmsg\n");
           msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
           exit(0);
        }
        if (mybuf.time != 0){
            printf("not null, time = %d\n", mybuf.time);
            int time = mybuf.time;
            sleep(time);
            sembuf.sem_op  = 1;
            semop(semid, &sembuf, 1);
        }
    }
}

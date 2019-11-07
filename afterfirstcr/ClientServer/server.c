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
typedef struct pair{
    int a;
    int b;
    int res;
    int id;
}pair_t;
#define LAST_MESSAGE 255
int main(){
    const int N = 5;
    int maxlen = 100;
    int msqid;
    char pathname[]="buf";
    int key;
    int len = 50;
    int n = 0;
    struct mymsgbuf
    {
       long mtype;
       pair_t pair;
    } mybuf;
    key = ftok(pathname, 0);
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    int n1 = 0;
    int tempN = N;
    int fork_ = 10;
    for(int i = 0; i < 5; i++){
        if (fork_ != 0)
            fork_ = fork();
    }
    while (1) {

       maxlen = 100;

       if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0){
         printf("Can\'t receive message from queue\n");
         exit(-1);
       }
       if (mybuf.mtype == LAST_MESSAGE) {
          msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
          exit(0);
       }
       // int fork_ = fork();
       // if (fork != 0){
       //     tempN--;
       //     if (tempN == 0){
       //      wait(NULL);
       //      //tempN = N;
       //      }
       // }
       // if (fork_ == 0){
           sleep(1);
           mybuf.mtype = mybuf.pair.id;
           mybuf.pair.res = mybuf.pair.a * mybuf.pair.b;
           if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
              printf("Can\'t send message to queue\n");
              msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
              exit(-1);
           }
           printf("message type = %ld, a = %d, b = %d, res = %d\n", mybuf.mtype, mybuf.pair.a, mybuf.pair.b, mybuf.pair.res);
          // tempN++;
          // exit(0);
       //}
    }

    return 0;
}

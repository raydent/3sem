#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct pair{
    int a;
    int b;
    int res;
    int id;
}pair_t;
int main(){
    int startid = getpid();
    int maxlen = 100;
    int msqid;
    char pathname[]="buf";
    int key;
    int len = 50;
    int n = 0;
    for(int i = 0; i < 20; i++){
        if (getppid() != startid)
            fork();
    }
    int id = getpid();
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
    mybuf.mtype = 1;
    mybuf.pair.a = 2;
    mybuf.pair.b = 3;
    mybuf.pair.id = id;
    maxlen = 100;
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, id, 0)) < 0){
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
    printf("a = %d, b = %d, res = %d\n", mybuf.pair.a, mybuf.pair.b, mybuf.pair.res);
    return 0;
}

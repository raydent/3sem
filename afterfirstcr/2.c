#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255
typedef struct {
    short sinfo;
    float finfo;
}info;
int main(void)
{
    int msqid, msqid2;
    char pathname[]="09-1a.c";
    char pathname1[] = "2.c";
    key_t  key, key2;
    int len, maxlen;
    int n = 0;
    struct mymsgbuf
    {
       long mtype;
       int n;
    } mybuf;

    key = ftok(pathname, 0);

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    if ((msqid2 = msgget(key2, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
       maxlen = 121;
       if (mybuf.mtype != LAST_MESSAGE){
           n = mybuf.n;
       }
       if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0){
         printf("Can\'t receive message from queue\n");
         exit(-1);
       }
       printf("message type = %ld, n = %d\n", mybuf.mtype, mybuf.n);
    int fac = 1;
    while(n != 1){
        fac *= n;
        n--;
    }

    mybuf.mtype = 1;
    mybuf.n = fac;

    len = 1 + sizeof(info);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
      printf("Can\'t send message to queue1\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }




    return 0;
}

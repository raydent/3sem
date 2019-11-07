#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LAST_MESSAGE 255


typedef struct {
    short sinfo;
    float finfo;
}info;

int main(void)
{

    int msqid, msqid2, maxlen;
    char pathname[]= "09-1a.c";
    char pathname1[] = "2.c";
    key_t  key, key2;
    int i,len;

    struct mymsgbuf
    {
       long mtype;
       int n;
    } mybuf;



    /* Create or attach message queue  */

    key = ftok(pathname, 0);

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }

    if ((msqid2 = msgget(key2, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }

    /* Send information */

       mybuf.mtype = 1;
       mybuf.n = 3;

       len = 1 + sizeof(info);

       if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to queue\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
       }

    /* Send the last message */
    sleep(3);
   if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0){
     printf("Can\'t receive message from queue\n");
     exit(-1);
   }
   int fac = mybuf.n;
     printf("fac = %d\n", fac);
    return 0;
}

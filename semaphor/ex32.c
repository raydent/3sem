#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
char state = 'a';
void writetolog(char state, struct sembuf mybuf, int fdef, int semid);

int main(int argc, char *argv[], char *envp[]){
    srand(time(NULL));
    int   semid;
    pid_t parent = getpid();
    char pathname[]="ex32.c";
    key_t key;
    struct sembuf mybuf;
    key = ftok(pathname, 0);
    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t create semaphore set\n");
      exit(-1);
    }
    mybuf.sem_num = 0;
    mybuf.sem_op = 1;
    mybuf.sem_flg = 0;
    semop(semid, &mybuf, 1);
    //int fileref = open("testfile.txt", O_RDONLY | O_APPEND);
    FILE* fil = fopen("testfile.txt", "w");
    fclose(fil);
    int filedesc = open("testfile.txt", O_WRONLY | O_APPEND);
    int f = 1;
    for (int i = 0; i < 1000; i++){
        if (getpid() == parent){
            f = fork();
        }
    }
    char state;
    if (f != 0){
        state = 'a';
        //wait(NULL);
    }
    else{
        state = (rand() % 20) + 'b';
    }
    writetolog(state, mybuf, filedesc, semid);
    //semctl(semid, IPC_RMID, NULL);
    // if(semop(semid, &mybuf, 1) < 0){
    //   printf("Can\'t wait for condition\n");
    //   exit(-1);
    // }
    if (getpid() == parent){
        wait(NULL);
        semctl(semid, IPC_RMID, 0);
    }
    //printf("The condition is present\n");
    return 0;
}
void writetolog(char state, struct sembuf mybuf, int fdef, int semid){
    mybuf.sem_num = 0;
    mybuf.sem_op = -1;
    semop(semid, &mybuf, 1);
    char a[11];
    memset(a, state, 10);
    a[10] = '\n';
    write(fdef, a, 11);
    mybuf.sem_op = +1;
    semop(semid, &mybuf, 1);
}

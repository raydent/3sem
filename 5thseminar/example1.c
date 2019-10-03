#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char** argv){
int fd, result;
size_t size;
char resstring[14];
char name01[]="aaaa.fifo";
char name10[] = "aaa.fifo";
(void)umask(0);

if(mknod(name01, S_IFIFO | 0666, 0) < 0);
int hh = mknod(name10, S_IFIFO | 0666, 0);
printf("hh = %d\n", hh);

if (strcmp(argv[1], "0") == 0){
    printf("0\n");
        int frk = fork();
    while(1){
        if (frk == 0){
            fd = open(name01, O_WRONLY);
            char buf[100];
            memset(buf, 0, 100);
            fgets(buf, 100, stdin);
            write(fd, buf, 100);
            memset(buf, 0, 100);
            close(fd);
        }
        else{

            fd = open(name10, O_RDONLY);
            char buf[100];
            memset(buf, 0, 100);
            read(fd, buf, 100);
            printf("%s", buf);
            memset(buf, 0, 100);
            close(fd);
        }
    }
    kill(getpid(), SIGKILL);
}
if (strcmp(argv[1], "1") == 0){
    printf("1\n");
    int frk = fork();
    while(1){
        int frk = fork();
        if (frk == 0){
            fd = open(name10, O_WRONLY);
            char buf[100];
            memset(buf, 0, 100);
            fgets(buf, 100, stdin);
            write(fd, buf, 100);
            memset(buf, 0, 100);
            close(fd);
        }
        else{
            fd = open(name01, O_RDONLY);
            char buf[100];
            memset(buf, 0, 100);
            read(fd, buf, 100);
            printf("%s", buf);
            memset(buf, 0, 100);
            close(fd);
        }
    }
}
return 0;
}

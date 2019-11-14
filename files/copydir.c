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
#include <pthread.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <dirent.h>
#include <sys/stat.h>

void copyfiles(char* target_dir_path, char* source_dir_path);

int main(){
    char* targetdir = "./tocopy";
    char* sourcedir = "./temp";
    copyfiles(targetdir, sourcedir);
}
void copyfiles(char* target_dir_path, char* source_dir_path){
    DIR* cur_target_dir = opendir(target_dir_path);
    DIR* cur_source_dir = opendir(source_dir_path);
    struct dirent *temp = readdir(cur_source_dir);
    char* name = (char*) calloc (100, sizeof(char));
    char* targetname = (char*) calloc (100, sizeof(char));
    struct stat buf;
    while(temp){
        strcpy(name, source_dir_path);
        strcat(name, "/");
        strcat(name, temp -> d_name);
        if (strcmp(temp -> d_name, ".") != 0 && strcmp(temp -> d_name, "..") != 0){
            stat(name, &buf);
            strcpy(targetname, target_dir_path);
            strcat(targetname, "/");
            strcat(targetname, temp -> d_name);
            if (S_ISREG(buf . st_mode)){
                FILE* sourcefile = fopen(name, "r");
                fseek(sourcefile, 0L, SEEK_END);
                int sz = ftell(sourcefile);
                rewind(sourcefile);
                char* file_content = (char*) calloc (sz, sizeof(char));
                fread(file_content, sizeof(char), sz, sourcefile);
                FILE* targetfile = fopen(targetname, "w");
                fwrite(file_content, sizeof(char), sz, targetfile);
                fclose(targetfile);
                fclose(sourcefile);
            }
            if (S_ISDIR(buf . st_mode)){
                mkdir(targetname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                copyfiles(targetname, name);
            }
        }
        temp = readdir(cur_source_dir);
    }
    free(name);
    free(targetname);
}

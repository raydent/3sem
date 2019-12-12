#include <sys/wait.h>
#include <string.h>
#include <time.h>

const int N = 100;

typedef struct mystruct {
    char* commandline;
    char** args;
    int waittime;
    int argnum;
}container_t;

int getargnum(char* string);
char** makeargarr(char* string);
container_t* fillcontainer(int stringnum , FILE* f);
char ** betterparse(char* string)
{
	int l = 0;
	int k = 0;
	int check = 0;
	char** result = (char**) calloc (100, sizeof(char*));
	for (int i = 0; i < 100; i++)
	{
	result[i] = (char*) calloc (100, sizeof(char)); 
	}
	for(;;)
        {
            for(int j = 0;;j++)
            {
                //fscanf(rFile, "%c", &Exe[i].command[k][j]);
		result[k][j] = string[l];
		l++;
                if (result[k][j] == ' ' && j == 0)
                {
                    j--;
                    continue;
                }

                if (result[k][j] == ' ')
                {
                    result[k][j] = '\0';
                    k++;
                    break;
                }
                else if (result[k][j] == '\n')
                {
                    result[k][j] = '\0';
                    result[k + 1] = (char *)0;
                    check = 1;
                    break;
                }
            }
            if (check)
                break;
        }
	return result;
}
char** Split(char* string, char* delimiters, int* tokensCount){
  int count = 10;
  char** tokens = (char**) calloc (count, sizeof(char*));
  printf("0\n");
  char *substring = strtok (string, delimiters);
  tokens[0] = substring;
  if (tokens[0] == NULL){
    return tokens;
  }
  *tokensCount += 1;
  while(substring != NULL){
    printf("1\n");
    substring = strtok(NULL, delimiters);
    tokens[*tokensCount] = substring;
    *tokensCount += 1;
    if(*tokensCount == count - 1){
      count *= 2;
      tokens = realloc(tokens, count * (sizeof(char*)));
    }
  }
  return tokens;
}

int main(){
    time_t t = time(NULL);
    int stringnum = 0;
    FILE* f = fopen("source.txt", "r");
    fscanf(f, "%d", &stringnum);
    char* buf = (char*) calloc (N, sizeof(char*));
    fgets(buf, N, f);
    //stringnum = (buf[0] - 9);
    container_t* container = fillcontainer(stringnum , f);
    int frk = 1;
    for (int i = 0; i < stringnum; i++) {
		//if (frk == 0) {
		//	kill(getpid(), SIGKILL);
		//}
		if (frk != 0) {
			frk = fork();
			if (frk == 0)
			{
				int frk1 = fork();
				int status = 1;
				if (frk1 != 0) {
					//t = time(NULL);
					for (;;)
					{
						//printf("time = %d\n" , time(NULL));
						if (abs(time(NULL) - t) > 5)
						{
							int status = 0;
							int stat = waitpid(frk1, &status, WNOHANG);
							int DIE = 1;
							DIE = kill(frk1, SIGTERM);
							//printf("DIE = %d\n", DIE);						
							DIE = kill(frk1, SIGKILL);						
							if (stat == 0) {
							printf("PROCESS %d TERMINATED\n", i);
							}
							//exit(0);
							//printf("dead %d\n", DIE);
							exit(0);
							//kill(getpid(), SIGKILL);
						}
					}
				}
				if (frk1 == 0 && status == 1)
				{	
					status = 0;
					sleep(container[i].args[0][0] - '0');
					status = execvp(container[i].args[1], container[i].args + 1);
					//kill(frk, SIGKILL);
					frk1 = 0;
					//kill(getppid(), SIGKILL);
					printf("error\n");
					exit(0);
				}
				if (frk1 == 0)
					exit(0);
			}
		}
    }
    exit(0);
    for(int i = 0; i <= container[stringnum - 1].argnum; i++){
    }
    /*if (frk > 0){
        wait(NULL);
        sleep(container[stringnum - 1].waittime);
        if (abs(time(NULL) - t) > 5){
            printf("process number %d was terminated\n", stringnum);
            kill(getpid(), SIGKILL);
        }
        execvp(container[stringnum - 1].args[1], container[stringnum - 1].args + 1);
    }*/
    fclose(f);
}
int getargnum(char* string){
    char* space = string;
    int n = 0;
    while(space != NULL) {
        space = strstr(space + 1, " ");
        n++;
    }
    return n;
}
char** makeargarr(char* string){
    int argnum = getargnum(string);
    char** argarr = (char**) calloc (argnum + 1, sizeof(char*));
    char *p1, *p2;
    p1 = string;
    p2 = string;
    int i = 0;
    while(p2 != NULL){
        p2 = strstr(p2, " ");
        if (p2 == NULL && p1 != NULL){
            argarr[i] = (char*) calloc (strlen(string) - (p1 - string), sizeof(char));
            strcpy(argarr[i], p1);
            break;
        }
        argarr[i] = (char*) calloc (p2 - p1, sizeof(char));
        strncpy(argarr[i], p1, p2 - p1);
	if (strcmp(argarr[i], "\n") == 0)
	{
		argarr[i] = NULL;
	}
        p2++;
        if(p2 == NULL)
            break;
        p1 = p2;
        i++;
    }
    argarr[i + 1] = NULL;
    return argarr;
}
container_t* fillcontainer(int stringnum , FILE* f){
    container_t* container = (container_t*) calloc (stringnum, sizeof(container_t));
    for(int i = 0; i < stringnum; i++){
        container[i].commandline = (char*) calloc (N, sizeof(char));
        fgets(container[i].commandline, N, f);
        //container[i].args = makeargarr(container[i].commandline);
	//container[i].args = betterparse(container[i].commandline);
	int trash = 0;
	container[i].args = Split(container[i].commandline, " \n", &trash);
        container[i].argnum = getargnum(container[i].commandline);
        container[i].waittime = (int)(container[i].commandline[0] - '0');
    }
    return container;
}

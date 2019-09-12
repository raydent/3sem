#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** Split(char* string, char* delimiters, int* tokensCount);
int main(){
  char** tokens = NULL;
  int tokensCount = 0;
  char* string = (char*) calloc (100, sizeof(char));
  strcpy(string, "Mama mila ramy, da");
  tokens = Split(string, " , ", &tokensCount);
  for(int i = 0; i < tokensCount; i++){
    if (tokens[i] != NULL){
      printf("%s\n", tokens[i]);
    }
  }
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

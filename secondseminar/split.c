#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** split(char* string, char symbol, int* wordnum);

int main(){
  char* mystring = "mama mila ramu";
  int wordnum = 0;
  char** stringArr = split(mystring, 'm', &wordnum);
  for(int i = 0; i < wordnum; i++){
    printf("%s\n", stringArr[i]);
    free(stringArr[i]);
  }
  free(stringArr);
}

char** split(char* string, char symbol, int* wordnum){
  char* symbolToString = (char*) calloc (1, sizeof(char));
  symbolToString[0] = symbol;
  while(string[0] == symbol){
    string++;
  }
  printf("%s\n", string);
  int count = 0;
  char* tempstring = string;
  while(strstr(tempstring, symbolToString)){
    tempstring = strstr(tempstring, symbolToString);
    if (tempstring - string >= strlen(string)){
      break;
    }
    count++;
    tempstring++;
  }
  printf("%d\n", count);
  char** stringarr = (char**) calloc (count + 1, sizeof(char*));
  char* curpoint = string;
  tempstring = string;
  for (int i = 0; i < count; i++){
    tempstring = strstr(curpoint, symbolToString) + 1;
    stringarr[i] = (char*) calloc (tempstring - curpoint - 1, sizeof(char));
    strncpy(stringarr[i], curpoint, tempstring - curpoint - 1);
    curpoint = tempstring;
  }
  if(curpoint - string != strlen (string)){
    stringarr[count] = (char*) calloc (strlen(string) - (tempstring - string), sizeof(char));
    strncpy(stringarr[count], curpoint, strlen(string) - (tempstring - string));
  }
  *wordnum = count + 1;
  // free(tempstring);
  // free(curpoint);
  return stringarr;
}

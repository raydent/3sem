#include <stdio.h>
int checkLeapYear(int year);
int main(){
  int year = 0;
  scanf("%d", &year);
  int answer = checkLeapYear(year);
  if (answer == 1)
    printf("Leap year\n");
  else
    printf("Common year\n");
}
int checkLeapYear(int year){
  if (year % 400 == 0)
    return 1;
  else if (year % 100 == 0)
    return 0;
  else if (year % 4 == 0)
    return 1;
  return 0;
}

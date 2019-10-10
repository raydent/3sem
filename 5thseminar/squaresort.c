#include <stdio.h>
#include <string.h>
typedef int (*MyFunc)(int, int);
int desc(int x, int c) {
    return (x - c);
}
int asc(int x, int c) {
    return (c - x);
}
void f(MyFunc func,  int* arr, int length) {
    printf("%d\n", length);
    for (int i = 0; i < length; i++){
        int m = i;
        for (int j = i + 1; j < length; j++){
            if (func(arr[j], arr[m]) > 0){
                m = j;
            }
            int temp = arr[i];
            arr[i] = arr[m];
            arr[m] = temp;
        }
    }
}
int main(){
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    f(asc, a, 10);
    for(int i = 0; i < 10; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

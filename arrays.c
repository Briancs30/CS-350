#include<stdio.h>

int a[10];

int main(int argc[], char const *argv[]){

    for(int i=0; i<10; i++){
        a[i] = i*i;
    }

    for(int i=0; i<10; i++){
        printf("a[%d] = %d\n", i ,a[i]);
    }
}
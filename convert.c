#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    //int x = 4.5;
    //printf("x = %d\n", x); x=4

    // Int to String
    int x = 123456;
    char buf[1000];
    //sprintf(buf, "%#x", x);//convert int to hex
    sprintf(buf, "%d", x);
    printf("x = %s\n", buf);

    // String to Int
    char num[20] = "38560";
    int val = atoi(num);
    printf("val = %d\n",val);
    printf("val = %d\n",val+5);
    //atof: float; atol: long

    printf("sizeof(int) = %ld\n", sizeof(int));
    return 0;
}
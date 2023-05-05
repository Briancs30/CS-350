#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    //argv is an array of strings
    //argv[0] is a string
    //argv[1] is a string
    // ...

    /*
    for(int i = 0; i<argc; i++)
        printf("%s\n", argv[i]);
    
    */
    int mychar = 0;
    char buf[1000];
    int i = 0;
    while((mychar = getchar()) != EOF){
        buf[i++] = mychar;
    }
    buf[i] = '\0';

    printf("\nSting = %s\n", buf);


    return 0;
}
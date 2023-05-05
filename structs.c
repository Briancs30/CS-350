#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct time{
  char hrs;
  short minutes;
  int seconds;
  char *time_string;
  char other_string[100];
}time;

typedef int my_size;


int main(int argc, char const *argv[])
{
  my_size x =10;
  printf("x= %d\n", x);
  
  //struct time my_time;
  //struct time *ptr;
  time my_time;
  time *ptr;
  
  ptr = (struct time*) malloc(sizeof(struct time));
  
  printf("sizeof(my_time) = %lu\n", sizeof(my_time));
  printf("sizeof(ptr) = %lu\n", sizeof(ptr));
  
  my_time.hrs = 10;
  my_time.minutes = 34;
  strcpy(my_time.other_string, "Here is some other string");
  
  my_time.time_string = (char*) malloc(100*sizeof(char));
  strcpy(my_time.time_string, "Here is a time string");
  
  printf("mytime.hrs = %d\n", my_time.hrs);
  printf("mytime.time_string = %s\n", my_time.time_string);
  
  (*ptr).hrs = 12;
  printf("(*ptr).hrs = %d\n", (*ptr).hrs);
  
  ptr->hrs = 12;
  printf("ptr->hrs = %d\n", ptr->hrs);
  return 0;
}
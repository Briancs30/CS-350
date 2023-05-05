#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char const *argv[])
{
  char* string = (char*)malloc(100*sizeof(char));
  strcpy(string, "Hello World!");
  printf("Message: %s\n", string);
  free(string);
  
  string = (char*)malloc(1000*sizeof(char));
  strcpy(string, "Here is a new Message");
  printf("Message: %s\n", string);
  free(string);
  
  return 0;
}
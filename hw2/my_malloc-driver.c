//
//  my_malloc-driver.c
//  Lab1: Malloc
//


#include "my_malloc.h"
#include "stdio.h"
#include "unistd.h"

typedef struct charcharint{
  char c1;
  char c2;
  int i;
}*cci;

int main(int argc, const char * argv[])
{
  char *ptr = sbrk(4096);
  cci x = (cci) ptr;
  cci y = x + 32; // need to be aligned by 8
  x -> c2 = 5;
  x -> i = 20;
  y -> i = 10;
  
  printf("c2 = %d\n", x -> c2);
  printf("i = %d\n", y -> i);
  return 0;
}

#include "my_malloc.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

uint32_t max(uint32_t a, uint32_t b, uint32_t c)
{
  if(a >= b && a >= c) return a;
  else if(b>= a && b>= c) return b;
  else return c;
}
int main(int argc, char const *argv[])
{
  uint32_t a = 6;
  uint32_t b = 6;
  uint32_t c = 1;
  printf("%d\n",max(a,b,c));
}
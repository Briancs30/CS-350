#include<stdio.h>
#include<stdlib.h>

#define LEN 10

//creates an array of specified length filled in with
// {1,2,3,...,length}
int* generate(int length){
  int *arr = (int*) malloc(length*sizeof(int));
  
  
  for(int i=0; i<length; i++){
    arr[i]=i+1;
  }
  return arr;
}

int main(int argc, char const *argv[]){
  int *ptr;
  ptr = generate(LEN);
  
  for(int i=0;i<LEN;i++){
    printf("%4d", ptr[i]);
  }
  free(ptr);
  printf("\n");
  return 0;
}
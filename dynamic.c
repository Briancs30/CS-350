#include<stdio.h>
#include<stdlib.h>

#define ROWS 2
#define COLS 3

int main(int argc, char const *argv[])
{
  /*
  int *a[2];
  a[0] = (int*)malloc(3*sizeof(int)); 
  a[1] = (int*)malloc(3*sizeof(int));
  
  for(int i=0;i<2;i++){
    for(int j=0;j<3;j++){
      printf("%4d", a[i][j]);
    }
    printf("\n");
  }
  */
  int **a = (int**) malloc(ROWS *sizeof(int*));
  a[0] = (int*) malloc(COLS*sizeof(int));
  a[1] = (int*) malloc(COLS*sizeof(int));
  
  //initializing 2D Array
  for(int i=0;i<ROWS;i++){
    for(int j=0;j<COLS;j++){
      a[i][j]= 1+i+j;
    }
  }
  
  // Prints the data of the 2D Array
  for(int i=0;i<ROWS;i++){
    for(int j=0;j<COLS;j++){
      printf("%4d", a[i][j]);
    }
    printf("\n");
  }
  
  printf("a = %p\n",a);
  printf("a[0] = %p\n",a[0]);
  printf("a[1] = %p\n",a[1]);
  
  for(int j=0;j<ROWS;j++){
    free(a[j]);
  }
  free(a);
}
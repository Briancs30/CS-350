#include<stdio.h>
int sum_array(int *array, int a);
int ptr_sum(int *address_of_array, int length);

#define ARRAY_LENGTH 10

int main(int argc, char const *argv[])
{
  int arr[ARRAY_LENGTH];
  for(int i = 0; i<ARRAY_LENGTH; i++){
    arr[i] = i;
  }
  int sum = sum_array(arr, ARRAY_LENGTH);
  printf("sum = %d\n", sum);
  int ptrsum= ptr_sum(arr, ARRAY_LENGTH);
  printf("ptr_sum = %d\n", ptrsum);
  return 0;
}

int sum_array(int *array, int a){
  int sum = 0;
  for(int i = 0; i<a; i++){
    sum += array[i];
  }
  return sum;
}

int ptr_sum(int *address_of_array, int length){
  int *ptr = address_of_array; //pointing at the start of array
  int sum =0;
  for(int i = 0; i < length; i++){
    sum += *ptr;
    ptr++;
  }
  return sum;
}

//2d arrays: arr+ i * ncols * sizeof(int)+ j* sizeof(int)
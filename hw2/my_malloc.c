/* THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. _Brian Hsu_ */
//
//  my_malloc.c
//  Lab1: Malloc
//

#include "my_malloc.h"
#include<unistd.h>

#define MINCHUNKSIZE 16
#define SIZEENLARGE 8192

FreeListNode first_list = NULL;
void *end_heap = NULL;
MyErrorNo my_errno=MYNOERROR;


FreeListNode find_freelist_space(uint32_t size)//find freelist to fit size
{
  FreeListNode found = NULL;
  if(first_list == NULL)
  {
    return NULL;
  }else if(first_list -> size >= CHUNKHEADERSIZE + size)
  {
    found = first_list;
    first_list = first_list -> flink;
    return found;
  }
  
  FreeListNode prev = first_list;
  while(prev -> flink != NULL)//loop & find freelist with space larger than requested
  {
    if(prev -> flink -> size >= CHUNKHEADERSIZE + size)
    {
      break;  
    }
    else
    {
      prev = prev -> flink;
    }
  }
  
  if (prev -> flink != NULL)
  {
    found = prev -> flink;
    prev -> flink = found -> flink;
    return found;
  }
  else
  {
    return NULL; //NULL if not found
  }
}

uint32_t max(uint32_t a, uint32_t b)
{
  if(a >= b) return a;
  else return b;
}

void add_freelistnode(void* chunkhead,uint32_t size)//add splited chunk back to freelist
{
  FreeListNode node = (FreeListNode) chunkhead;
  node -> size = size;
  if(first_list == NULL)
  {
    first_list = node;
    node -> flink = NULL;
    return;
  }
  else if(first_list > node)
  {
    node -> flink = first_list;
    first_list = node;
    return;
  }
  FreeListNode prev = first_list;
  while(prev -> flink != NULL)//found available spaces
  {
    if(prev -> flink > node)
    {
      node -> flink = prev -> flink;
      prev -> flink = node;
      return;
    }
    else
    {
      prev = prev -> flink;
    }
  }
  prev -> flink = node;
  node -> flink = NULL;
  return;
}

void magic_number(void * chunkHead,uint32_t updated_size)
{
  uint32_t * ptr = (uint32_t *) chunkHead;
  *ptr++ = updated_size;
  *ptr = 0xfffff350;// magic number
}

void *my_malloc(uint32_t size)//returns pointer
{
  FreeListNode node= NULL; //tracks available heap memory
  void *chunkHead = NULL;
  size_t updated_size = 0;
  if(end_heap == NULL)
  {
    end_heap = sbrk(0);
  }
  if(size % CHUNKHEADERSIZE)
  {
    size += CHUNKHEADERSIZE - size % CHUNKHEADERSIZE;
  }
  uint32_t rounded_freelistnode = sizeof(struct freelistnode);
  size = max(size, (max(rounded_freelistnode, MINCHUNKSIZE) - CHUNKHEADERSIZE));
  
  //get space from freelist and check valid
  if((node = find_freelist_space(size)) == NULL)
  {
    if(size <= SIZEENLARGE - CHUNKHEADERSIZE)
    {
      if((chunkHead = sbrk(SIZEENLARGE)) == (void *)-1)//failure case
      {
        my_errno = MYENOMEM;
        return NULL;
      }
      
      //split space and save unused space back to freelist
      uint32_t sizeremaining = SIZEENLARGE - CHUNKHEADERSIZE - size;
      if(sizeremaining >= max(rounded_freelistnode, MINCHUNKSIZE)){
        void * splitchunk = chunkHead + size + CHUNKHEADERSIZE;
        add_freelistnode(splitchunk, sizeremaining);
        updated_size = size + CHUNKHEADERSIZE;
      }
      else
      {
        updated_size = SIZEENLARGE;
      }
    }
    else
    {
      if((chunkHead = sbrk(size + CHUNKHEADERSIZE)) == (void*) -1)//failure case
      {
        my_errno = MYENOMEM;
        return NULL;
      }
      updated_size = size + CHUNKHEADERSIZE;
    } 
  }
  else//space valid case
  {
    chunkHead = (void*) node;
    //split space and save unused space back to freelist
    uint32_t sizeremaining = node -> size - CHUNKHEADERSIZE - size;
    if(sizeremaining >= max(rounded_freelistnode, MINCHUNKSIZE))
    {
      void * splitchunk = chunkHead + size + CHUNKHEADERSIZE;
      add_freelistnode(splitchunk, sizeremaining);
      updated_size = size + CHUNKHEADERSIZE;
    }
    else
    {
      updated_size = node -> size;
    }
  }
  magic_number(chunkHead, updated_size);//magic number
  my_errno = MYNOERROR;
  return chunkHead + CHUNKHEADERSIZE;
}
      
void my_free(void *ptr)//free the pointed space
{
  if(ptr > sbrk(0) || end_heap == NULL || end_heap > (ptr - CHUNKHEADERSIZE))//identify error cases
  {
    my_errno = MYBADFREEPTR;
    return;
  }
  ptr -= CHUNKHEADERSIZE;
  uint32_t * ptr_head = (uint32_t *) ptr;
  if(*++ptr_head != 0xfffff350)//check magic number
  {
    my_errno = MYBADFREEPTR;
    return;
  }
  add_freelistnode(ptr, *--ptr_head);//add space back to freelist
  my_errno = MYNOERROR;
}

FreeListNode free_list_begin()//return first freelist chunk
{
  return first_list;
}

void coalesce_free_list()//combine chunks on freelist together if next to each other
{
  FreeListNode node = first_list;
  while(node != NULL)
  {
    if((void *) node + node -> size == node -> flink)
    {
      node -> size += node -> flink -> size;
      node -> flink = node -> flink -> flink;
    }
    else
    {
      node = node -> flink;
    }
  }
}
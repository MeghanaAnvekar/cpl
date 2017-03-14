#include<stdio.h>
# define MIN1 0
# define MAX1 4999
# define MIN2 5000
# define MAX2 9999

typedef struct f_node
{
  int start;
  int size;
  struct f_node * next;
}F_node;

typedef struct a_node
{
  char name[33];
  int start;
  int size;
  struct a_node * next;
}A_node;

A_node *active_AL=NULL,*passive_AL=NULL;
F_node *active_FL=NULL,*passive_FL=NULL;

void init_active();
void init_passive();
void mem_alloc(char *name,int size);
void mem_free(char* name);
void mem_compact();
A_node* make_A_node(char* name,int start,int size);
F_node* make_F_node(int start,int size);
void delFreeList();
void delAllocatedList();

void init_active()
{
  active_FL = make_F_node(MIN1,MAX1+1);
  active_AL = NULL;
}

void init_passive()
{
  passive_FL = make_F_node(MIN2,MAX2+1);
  passive_AL = NULL;
}

A_node* make_A_node(char* name,int start,int size)
{
  A_node *nptr = (A_node*)malloc(sizeof(A_node));
  strcpy(nptr->name,name);
  nptr->start = start;
  nptr->size = size;
}
F_node* make_F_node(int start,int size)
{
  F_node *nptr = (F_node*)malloc(sizeof(F_node));
  nptr->start = start;
  nptr->size = size;
}
void mem_alloc(char *name,int size)
{
  F_node *fptr = active_FL,*prev;
  A_node *aptr = active_AL,*hold;
  int sum = 0;

  int done = 0;

  if(!aptr)
  {
    if(fptr->size >= size)
    {
      start = fptr->start;
      fptr->start +=size;
      fptr->size -= size;
      done = 1;
    }
    else
    {
      sum = fptr->size;
    }
  }
  else
  {
    prev = fptr;
    fptr = fptr->next;
    int found = 0;
    while(fptr && !done && !found)
    {
      if(fptr->size >= size && !found)
      {
        while(aptr && !found)
        {
          if(strcmp(aptr->name,name) == 0)
          {
            found = 1;
            printf("\nERROR: Heap element with that name already exists !.\n");
          }
          hold = aptr;
          aptr = aptr->next;
        }

        if(!found)
        {
          start = fptr->start;
          fptr->size -= size;

          if(fptr->size == 0)
          {
            prev->next = fptr->next;
            free(fptr);
          }
          else
          {
            fptr->start += size;
          }
        }
      }
      else
      {
        sum += fptr->size;
        prev = fptr;
        fptr = fptr->next;
      }
    }
  }

  if(!done)
  {
    if(aptr)
    {
      aptr->next = make_A_node(name,fptr->start,size);
    }
    else
    {
      active_AL = make_A_node(name,fptr->start,size);
    }

  }
  else
  {
    if(size <= sum)
    {
      mem_compact();
      mem_alloc(name,size);
    }
    else
    {
      printf("\nSorry, You are short of memory !\n");
    }
  }
}

void mem_free(char* name)
{
  int found = 0;
  A_node *aptr ,*prev ;

  if(active_AL)
  {
    aptr = prev = active_AL;
    while(aptr != NULL && !found)
    {
      if(strcmp(aptr->name,name) == 0)
      {
        found = 1;
      }
      else
      {
        prev = aptr;
        aptr = aptr->next;
      }

    }

    if(!found)
    {
      printf("\nInvalid call to free heap element !.\n");
    }
    else
    {
      F_node *nptr = make_F_node(start,size);
      nptr->next = active_FL;
      active_FL = nptr;
    }
  }
}

void mem_compact()
{
  init_compact();
  F_node *fptr1 = active_FL ,*fptr2 = passive_FL;
  A_node *aptr2 = passive_AL,*aptr1 = active_AL;

  if(!aptr2)
  {
    passive_AL = make_A_node(aptr1->name, fptr2->start,aptr1->size);
    fptr2->start += aptr1->size;
    fptr2->size -= aptr1->size;
    aptr1 = aptr1->next;
  }
  else
  {
    aptr2 = passive_AL;
    while(aptr1)
    {
      aptr2->next = make_A_node(aptr1->name,fptr2->start,aptr1->size);
      fptr2->start += aptr1->size;
      fptr2->size -= aptr1->size;
      aptr1 = aptr1->next;
      aptr2 = aptr2->next;
    }

    delFreeList();
    delAllocatedList();

  }

}

void delFreeList()
{
  F_node *ptr = active_FL,*temp;

  while(ptr)
  {
    temp = ptr;
    ptr = ptr->next;
    free(temp);
  }
}
void delAllocatedList()
{
  A_node *ptr = active_AL,*temp;

  while(ptr)
  {
    temp = ptr;
    ptr = ptr->next;
    free(temp);
  }
}

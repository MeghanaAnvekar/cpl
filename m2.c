#include<stdio.h>
#include<string.h>
#include<stdlib.h>
# define MIN 0
# define MAX 1999


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

A_node *allocated_list=NULL;
F_node *free_list=NULL;
int ctr = 0;

void init(int,int);
void mem_alloc(char *name,int size);
void mem_free(char* name);
A_node* make_A_node(char* name,int start,int size);
F_node* make_F_node(int start,int size);
void delFreeList();
void delAllocatedList();
void place_freed_node(int start, int size);
int A_search(char *name);

void traverse_free_list(F_node*);
void traverse_allocated_list(A_node*);

void main()
{
  int i,size,flag = 1;
  char name[33];
  int choice;
  init(MIN,MAX);
  printf("\n*************************************************************\n");
  printf("                   HEAP STORAGE MANAGEMENT ");
  printf("\n*************************************************************\n");
  
  traverse_free_list(free_list);
		traverse_allocated_list(allocated_list);

  for(i = 0;i < 100 && flag;++i)
  {
    while(flag)
    {
      printf("\n\nMENU :\n");
      printf("1. Allocate\n2. Free \n3. Quit\nEnter your choice....");
      scanf("%d",&choice);
	choice = (int)choice;
      switch(choice)
      {
        case 1: printf("\n============================================================================\n");
        	printf("\nVARIABLE NAME : ");
                scanf("%s",name);
                printf("\nSIZE (in bytes): ");
                scanf("%d",&size);
                mem_alloc(name,size);
                traverse_free_list(free_list);
		traverse_allocated_list(allocated_list);
		 printf("\n============================================================================\n");
                break;
        case 2: printf("\n============================================================================\n");
        	printf("\nVARIABLE NAME : ");
                scanf("%s",name);
                mem_free(name);
                traverse_free_list(free_list);
		traverse_allocated_list(allocated_list);
		 printf("\n============================================================================\n");
                break;

        case 3: printf("\n\n*******************PROGRAM TERMINATED******************\n\n");
                  flag = 0;
                  break;
        default: printf("\nInvalid choice!!!.");
                  break;
      }
  }
    
  }

}

void init(int min,int max)
{
  free_list = make_F_node(min,max+1);
  allocated_list = NULL;
}

A_node* make_A_node(char* name,int start,int size)
{
  A_node *nptr = (A_node*)malloc(sizeof(A_node));
  strcpy(nptr->name,name);
  nptr->start = start;
  nptr->size = size;
  nptr->next = NULL;
}
F_node* make_F_node(int start,int size)
{
  F_node *nptr = (F_node*)malloc(sizeof(F_node));
  nptr->start = start;
  nptr->size = size;
  nptr->next = NULL;
}
void mem_alloc(char *name,int size)
{
  F_node *fptr = free_list,*prev;
  A_node *aptr = allocated_list,*nptr;
  int msg = 1;

  int found = 0;

	if(fptr == NULL)
	{
		printf("\n Memory is FULL.");
	}
	else
	{
		while(fptr && !found)
		{
			if(fptr->size >= size)
			{
				if(A_search(name))
				{	
					msg = 0;
					printf("\n%s is already allocated !.\n",name);
					break;
				}
				nptr = make_A_node(name,fptr->start,size);
				fptr->size -= size;
				fptr->start += size;
			
				if(!fptr->size)
				{
					if(fptr == free_list )
					{
						if(fptr->next == NULL)
							free_list = NULL;
						else
							free_list = fptr->next;
					}
					else
					{
						prev->next = fptr->next;	
					}
					free(fptr);
				}
				found = 1;
			}
			else
			{
				prev = fptr;
				fptr = fptr->next;
			}
		}
	
		if(found)
		{
			nptr->next = allocated_list;
			allocated_list = nptr;
		}
		else
		{
			if(msg)
			printf("\n Sorry !Your are short of memory!\n");
		}	
	}

	  
}

void mem_free(char* name)
{
  int found = 0;
  A_node *aptr = allocated_list ,*prev ;
  F_node *fptr,*nptr;

  if(!aptr)
  {
  	printf("\nERROR: Memory is Empty!....You are trying to perform Illegal free operation\n");
  }
  else
  {
  	do
  	{
  		if(!strcmp(aptr->name,name))
  		{
  			found = 1;
  		}
  		else
  		{
  			prev = aptr;
  			aptr = aptr->next;
  		}
  	}
  	while(aptr && !found);
  	
  	if(found)
  	{
  		if(aptr == allocated_list)
  		{
  			allocated_list = aptr->next;
  		}
  		else
  		{
  			prev->next = aptr->next;
  		}
  		place_freed_node(aptr->start,aptr->size);
  		free(aptr);
  		
  	}
  	else
  	{
  		printf("\nERROR: You are trying to perform Illegal free operation\n");	
  	}
  }
}

void delFreeList()
{
  F_node *ptr = free_list,*temp;

  while(ptr)
  {
    temp = ptr;
    ptr = ptr->next;
    free(temp);
  }
}
void delAllocatedList()
{
  A_node *ptr = allocated_list,*temp;

  while(ptr)
  {
    temp = ptr;
    ptr = ptr->next;
    free(temp);
  }
}

 void place_freed_node(int start, int size)
 {
	 F_node *fptr = free_list,*prev=NULL;
	 int done = 0;
	 

	F_node *nptr = make_F_node(start,size);

	 if(fptr)
	 {
		do
		{
			//coalesce adjacent free blocks
			if(fptr->start + fptr->size == nptr->start )
			{
				nptr->size = fptr->size+nptr->size;
				nptr->start = fptr->start;
				done = 1;
	
			}
			else if(nptr->start + nptr->size == fptr->start)
			{
				nptr->size = fptr->size+nptr->size;
				done = 1;
				
			}
			else
			{
				prev  = fptr;
				fptr = fptr->next;
			}
			
			if(done)
			 {
			 	if(prev == NULL)
				{
					free_list = fptr->next;
					free(fptr);
					fptr = free_list;
				}
				else
				{
					prev->next = fptr->next;
					free(fptr);
					fptr = prev->next;
				}
				
			 }
			
		}while(fptr);
	 }
	 //if adjacent blocks in free list were not found or free list was empty
	nptr->next = free_list;
	free_list = nptr;
	 
 }
 
 int A_search(char *name)
 {
 	A_node * aptr = allocated_list;
 	int found = 0;
 	if(aptr)
 	{ 		
 		do
 		{
 			if( !strcmp(name, aptr->name))
 			{
 				found = 1;
 			}
 			else
 			{
 				aptr = aptr->next;
 			}
 		}
 		while(aptr && !found);
 	}
 	
 	return found;
 }
   
void traverse_free_list(F_node* fptr)
{
	int i = 0,size = 0;
	printf("\n\nFREE LIST :\n");
	printf("S.no   Starting Address    Size(in bytes)\n");
	if(fptr)
	while(fptr)
	{
		++i;
		printf("%d.        %-5d                   %-5d\n",i,fptr->start,fptr->size);
		fptr = fptr->next;
	}
	else
	printf("1.        0                   0\n");
	
}
void traverse_allocated_list(A_node* aptr)
{
	int i = 0,size = 0;
	printf("\n\nALLOCATED LIST :\n");
	printf("S.no    Variable Name    Starting Address    Size(in bytes)\n");
	if(aptr)
	while(aptr)
	{
		++i;
		printf("%d.       %s                %-5d                    %-5d\n"      ,i,aptr->name,aptr->start,aptr->size);
		aptr = aptr->next;
	}
	else
	printf("1.       -                 0                    0\n");

}

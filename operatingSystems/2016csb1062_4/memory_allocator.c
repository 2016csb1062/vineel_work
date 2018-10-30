#include <unistd.h>
#include <stdio.h>
<<<<<<< HEAD
#include <string.h>

//functions declarations 
void *csl333_malloc(size_t size);
void* csl333_realloc(void* ptr, size_t size);
void csl333_free(void *ptr);


void * findFreedBlock(size_t size);
void printMetaData();
void zero_fill(void * ptr,size_t size);


//structure to store meta_data of a block
=======

void *csl333_malloc(size_t size);
void csl333_free(void *ptr);
void* csl333_realloc(void* ptr, size_t size);
void *csl333_malloc(size_t size);
void * findFreedBlock(size_t size);

>>>>>>> d6c99e383bf9c5a7f457e74f6aa033587ed14ec6
struct meta_data
{
	int is_free ;
	size_t size;
<<<<<<< HEAD
	struct  meta_data * next ;
	struct  meta_data * prev ;
};

//head and tail of the linked list;
struct meta_data * head;
struct meta_data * tail;


void *csl333_malloc(size_t size)
{
	if(size == 0)
		return NULL;

	void * freeBlock = findFreedBlock(size);

	if(freeBlock != NULL)
	{
		printMetaData();
		return freeBlock;
	}

=======
	struct  meta_data * next ; 
};

struct meta_data * head;
struct meta_data * tail;

void *csl333_malloc(size_t size)
{
	void * freeBlock = findFreedBlock(size);
	if(freeBlock != NULL)
	{
		return freeBlock;
	}
>>>>>>> d6c99e383bf9c5a7f457e74f6aa033587ed14ec6
	else
	{
		//create a temp Block
		size_t total_size = sizeof(struct meta_data)+size;
<<<<<<< HEAD
		void * tempBlock = sbrk(total_size);

		//zero_fill the block
		zero_fill(tempBlock,size);

		if(tempBlock ==  (void *) -1)
		{
			return NULL;
		}
=======
		printf("++++%d",sbrk(0));
		printf("++++%d",total_size);
		void * tempBlock = sbrk(total_size);
		printf("++++%d",sbrk(0));
		printf("++++%d",tempBlock);
		printf("++++%d",sbrk(0));
>>>>>>> d6c99e383bf9c5a7f457e74f6aa033587ed14ec6
		if(head == NULL)
		{
			head = tempBlock;
			head -> is_free = 0;
			head -> size = size;
			head -> next = NULL;
<<<<<<< HEAD
			head -> prev = NULL;
			tail = head;
			printMetaData();
=======
			tail = head;
>>>>>>> d6c99e383bf9c5a7f457e74f6aa033587ed14ec6
			return (void *)(head+1);
		}

		tail->next = tempBlock;
<<<<<<< HEAD
		tail->next->prev = tail;
=======
>>>>>>> d6c99e383bf9c5a7f457e74f6aa033587ed14ec6
		tail = tail->next;
		tail->is_free = 0;
		tail->size = size;
		tail->next = NULL;
<<<<<<< HEAD
		printMetaData();
=======

>>>>>>> d6c99e383bf9c5a7f457e74f6aa033587ed14ec6
		return (void *)(tail+1);

	}

}

void * findFreedBlock(size_t size)
{
<<<<<<< HEAD
	struct meta_data * temp_meta_data = head;
	while(temp_meta_data != NULL)
	{
		if(temp_meta_data->is_free == 1)
		{
			if(temp_meta_data->size>=size)
			{
				temp_meta_data->is_free = 0;

				//split

				if(temp_meta_data->size > sizeof(struct meta_data)+size)
				{
					struct meta_data * temp_meta_data1 = ((void *)(temp_meta_data+1))+size ;
					temp_meta_data1->is_free = 1;
					temp_meta_data1->size = temp_meta_data ->size - size - sizeof(struct meta_data);
					//printf("Found Split , splitPtr :%p,splitSize :%zu\n ",temp_meta_data1+1,temp_meta_data1->size);
					temp_meta_data1->next = temp_meta_data->next;
					if(temp_meta_data1->next!=NULL)
						temp_meta_data1->next->prev = temp_meta_data1;
					temp_meta_data1->prev = temp_meta_data;
					
					temp_meta_data->size = size;
					temp_meta_data->next = temp_meta_data1;
					return (void *) (temp_meta_data+1);
				}
				return (void *) (temp_meta_data+1);
			}
		}
				temp_meta_data = temp_meta_data->next;
	}
	return NULL;
}



void  csl333_free(void *ptr)
{
	//zero_fill the block
	if(ptr == NULL)
	{
		return ;
	}

	struct meta_data * temp_meta_data = (struct meta_data *) (ptr - sizeof(struct meta_data));
	temp_meta_data->is_free = 1;
	zero_fill(ptr,temp_meta_data->size);
	printMetaData();

	if(temp_meta_data->next!=NULL && temp_meta_data->next->is_free == 1)
	{
		temp_meta_data->size += temp_meta_data->next->size + sizeof(struct meta_data);
		temp_meta_data->next = temp_meta_data->next->next;
		if(temp_meta_data->next!=NULL)
			temp_meta_data->next->prev = temp_meta_data; 
	}
	if(temp_meta_data->prev!=NULL && temp_meta_data->prev->is_free == 1)
	{
		temp_meta_data = temp_meta_data->prev;
		temp_meta_data->size += temp_meta_data->next->size + sizeof(struct meta_data);
		temp_meta_data->next = temp_meta_data->next->next;
		if(temp_meta_data->next!=NULL)
			temp_meta_data->next->prev = temp_meta_data;	 
	}
	return;
}

void *csl333_realloc(void * ptr,size_t size)
{
	void * temp = csl333_malloc(size);
	if(temp != NULL && ptr!=NULL)
		memcpy(temp,ptr,size);
	csl333_free(ptr);
	return temp;
}


void printMetaData()
{
	struct meta_data * temp_meta_data = head;
	printf("--------Strating MetaData List-----------\n");
	while(temp_meta_data!=NULL)
	{
		printf("ptr:%p , size:%zu, is_free:%d\n",temp_meta_data,temp_meta_data->size,temp_meta_data->is_free);
		temp_meta_data=temp_meta_data->next;
	}
	printf("------------------------------------------\n\n\n");
}

void zero_fill(void * ptr,size_t size)
{
	memset(ptr , 0 , size);
}

int main(int argc, char const *argv[]) 
{
  int * a = csl333_malloc(sizeof(int));
  *a=2;
  a = csl333_realloc(a,2*sizeof(int));
  *(a+1) = 3;
  printf("%d%d",*a,*(a+1));
  return 0;
=======
	return NULL;
}

void  csl333_free(void *ptr)
{
	printf("-------%d\n",ptr);
	struct meta_data * temp_meta_data = (struct meta_data *) (ptr - sizeof(struct meta_data));
	printf("-------%d\n",ptr + temp_meta_data->size);
	if(ptr + temp_meta_data->size == sbrk(0))
	{
		sbrk(0-temp_meta_data->size-sizeof(struct meta_data));
		return;
	}
	temp_meta_data->is_free = 1;
	return;
}

int main()
{
	int * a = csl333_malloc(sizeof(int));
	*a = 9;
	printf("%d\n",*a);
	printf("%d\n",sbrk(0));
	csl333_free(a);
	printf("%d\n",sbrk(0));
>>>>>>> d6c99e383bf9c5a7f457e74f6aa033587ed14ec6
}
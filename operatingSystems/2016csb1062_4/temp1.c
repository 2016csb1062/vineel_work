#include <unistd.h>
#include <stdio.h>

void *csl333_malloc(size_t size);
void csl333_free(void *ptr);
void* csl333_realloc(void* ptr, size_t size);
void *csl333_malloc(size_t size);
void * findFreedBlock(size_t size);
void * splitBlock(void * ptr,size_t t);

struct meta_data
{
	int is_free ;
	size_t size;
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
	else
	{
		//create a temp Block
		size_t total_size = sizeof(struct meta_data)+size;
		void * tempBlock = sbrk(total_size);
		if(head == NULL)
		{
			head = tempBlock;
			head -> is_free = 0;
			head -> size = size;
			head -> next = NULL;
			tail = head;
			return (void *)(head+1);
		}

		tail->next = tempBlock;
		tail = tail->next;
		tail->is_free = 0;
		tail->size = size;
		tail->next = NULL;

		return (void *)(tail+1);

	}

}

void * findFreedBlock(size_t size)
{
	struct meta_data * temp_meta_data = head;
	while(temp_meta_data != NULL)
	{
		if(temp_meta_data->is_free == 1)
		{
			if(temp_meta_data->size>=size)
			{
				temp_meta_data = splitBlock(temp_meta_data,size);
				temp_meta_data->is_free = 0;
				return (void *) (temp_meta_data+1);
			}
		}
		temp_meta_data=temp_meta_data->next;
	}
	return NULL;
}

void * splitBlock(void * ptr,size_t size)
{
	struct meta_data * temp_meta_data = ptr;
	if(temp_meta_data->size-size > sizeof(struct meta_data))
	{
		printf("%zu",size);
		
		return (void *)(temp_meta_data+1);
	}
	return (void *)(temp_meta_data+1); 
}


void  csl333_free(void *ptr)
{
	struct meta_data * temp_meta_data = (struct meta_data *) (ptr - sizeof(struct meta_data));
	if(ptr + temp_meta_data->size == sbrk(0))
	{
		if(temp_meta_data == head)
			head = NULL;
		sbrk(0-temp_meta_data->size-sizeof(struct meta_data));
		return;
	}
	temp_meta_data->is_free = 1;
	return;
}

void* csl333_realloc(void* ptr, size_t size)
{

}

int main()
{
	void * a = csl333_malloc(2*sizeof(int)+2*sizeof(struct meta_data));
	void * b = csl333_malloc(sizeof(int));
	csl333_free(a);
	a = csl333_malloc(sizeof(int));
	void * c = csl333_malloc(sizeof(int));
	csl333_free(b);
	csl333_free(c);
	csl333_free(a);
	if(head == NULL)
		printf("Yeas!");
	return 0;
}
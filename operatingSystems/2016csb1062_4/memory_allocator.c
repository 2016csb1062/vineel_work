#include <unistd.h>
#include <stdio.h>

void *csl333_malloc(size_t size);
void csl333_free(void *ptr);
void* csl333_realloc(void* ptr, size_t size);
void *csl333_malloc(size_t size);
void * findFreedBlock(size_t size);

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
		printf("++++%d",sbrk(0));
		printf("++++%d",total_size);
		void * tempBlock = sbrk(total_size);
		printf("++++%d",sbrk(0));
		printf("++++%d",tempBlock);
		printf("++++%d",sbrk(0));
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
}
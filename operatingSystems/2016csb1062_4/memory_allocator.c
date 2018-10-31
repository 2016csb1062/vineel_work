#include <unistd.h>
#include <stdio.h>
#include <string.h>

//functions declarations 
void *csl333_malloc(size_t size);
void* csl333_realloc(void* ptr, size_t size);
void csl333_free(void *ptr);

//finds a free block of given size
void * findFreedBlock(size_t size);
void printMetaData();
void zero_fill(void * ptr,size_t size);


//structure to store meta_data of a block
struct meta_data
{
	int is_free ;
	size_t size;
	struct  meta_data * next ;
	struct  meta_data * prev ;
};

//head and tail of the linked list;
struct meta_data * head;
struct meta_data * tail;


void * csl333_malloc(size_t size)
{
	if(size == 0)
		return NULL;

	void * freeBlock = findFreedBlock(size);

	if(freeBlock != NULL)
	{
		//printMetaData();
		return freeBlock;
	}

	else
	{
		//create a temp Block
		size_t total_size = sizeof(struct meta_data)+size;
		void * tempBlock = sbrk(total_size);

		//zero_fill the block
		zero_fill(tempBlock,size);

		//if sbrk cannot extend heap
		if(tempBlock ==  (void *) -1)
		{
			return NULL;
		}

		//adding first time
		if(head == NULL)
		{
			head = tempBlock;
			head -> is_free = 0;
			head -> size = size;
			head -> next = NULL;
			head -> prev = NULL;
			tail = head;
			//printMetaData();
			return (void *)(head+1);
		}


		tail->next = tempBlock;
		tail->next->prev = tail;
		tail = tail->next;
		tail->is_free = 0;
		tail->size = size;
		tail->next = NULL;
		//printMetaData();
		return (void *)(tail+1);

	}

}

void * findFreedBlock(size_t size)
{
	struct meta_data * temp_meta_data = head;
	//traverse the linkedList
	while(temp_meta_data != NULL)
	{
		if(temp_meta_data->is_free == 1)
		{
			if(temp_meta_data->size>=size)
			{
				//keep the is_free tag to 0
				temp_meta_data->is_free = 0;


				//split
				if(temp_meta_data->size > sizeof(struct meta_data)+size)
				{
					struct meta_data * temp_meta_data1 = ((void *)(temp_meta_data+1))+size ;
					temp_meta_data1->is_free = 1;
					temp_meta_data1->size = temp_meta_data ->size - size - sizeof(struct meta_data);
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
	if(ptr == NULL)
	{
		return ;
	}

	struct meta_data * temp_meta_data = (struct meta_data *) (ptr - sizeof(struct meta_data));
	temp_meta_data->is_free = 1;
	zero_fill(ptr,temp_meta_data->size);
	//printMetaData();

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


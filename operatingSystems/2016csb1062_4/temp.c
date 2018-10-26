#include <stdio.h>

struct meta_data
{
	int is_free;
	size_t size;
	struct  meta_data * next ; 
};


int main()
{
	printf("%d",sizeof(struct meta_data));
}
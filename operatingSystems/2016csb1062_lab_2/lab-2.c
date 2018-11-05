#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char ** argv)
{
	if(argc!=3)
	{
		printf("only two values should be given,n and k !!\n");
		return -1;
	}
	int k = atoi(argv[1]);
	int f = atoi(argv[2]);
	size_t N = f*1024;
	printf("%zu\n",N);
	char * my_arr = malloc(N);
	int number_of_writes = (N)/k;
	
	char x = 'v';
	char * temp = my_arr;

	//write k byte at a time
	for(int i=0;i<number_of_writes;i++)
	{
		memset(temp,x,k*sizeof(char));
		temp[0] = '|';
		temp = temp+k;
		//printf("%p\n",temp);
	}
	temp = my_arr;
	char tempX[k];
	
	//read k bytes at a time
	for(int i=0;i<number_of_writes;i++)
	{
		memcpy(tempX,temp,k*sizeof(char));
		printf("%s\n",tempX);
	}

	return 0;
}

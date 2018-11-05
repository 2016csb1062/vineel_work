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
	int N = atoi(argv[2]);
	char * my_arr = malloc(N*1024);
	int number_of_writes = (N*1024)/k;
	char x = 'v';
	char * temp = my_arr;
	for(int i=0;i<number_of_writes;i++)
	{
		memset(temp,x,k);
		temp+=k;
	}
	for(int i=0;i<number_of_writes;i++)
	{
		printf("%c",my_arr[i]);
	}

	return 0;
}

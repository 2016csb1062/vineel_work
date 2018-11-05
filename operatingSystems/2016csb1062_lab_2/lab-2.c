#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

clock_t start, end;
double cpu_time_used;

int main(int argc,char ** argv)
{
	int n[]={10, 20, 40, 80, 160, 320, 640, 1280};
	int K[]={10, 20, 30, 40, 50, 60};

	double timeMat[8][6][4];
	char * my_arr;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<6;j++)
		{
			int k = atoi(argv[1]);
			int n = atoi(argv[2]);
			size_t N = n*1024*1024;
	
			//giving memory to array
			my_arr = malloc(N);

			//number of writes if k bytes are written at a time
			int number_of_writes = (N)/k;
	
			//elements of array
			char x = 'v';

			//write 1 byte at a time

			char * temp = my_arr;
			start = clock();
			for(int i=0;i<N;i++)
			{
				memset(temp,x,1*sizeof(char)); // wrtie 1 byte at a time;
				temp+=1;
			}
			end = clock();
			timeMat[i][j][0]=((double) (end - start)) / CLOCKS_PER_SEC;


			//write k byte at a time
			temp = my_arr;
			start = clock();
			for(int i=0;i<number_of_writes;i++)
			{
		
				memset(temp,x,k*sizeof(char)); // wrtie k bytes at a time;
				temp = temp+k;
			}
			end = clock();
			timeMat[i][j][1]=((double) (end - start)) / CLOCKS_PER_SEC;


			//read k bytes at a time
			temp = my_arr;
			char x_arr[k];
			start = clock();
			for(int i=0;i<number_of_writes;i++)
			{
				memcpy(x_arr,temp,k*sizeof(char));
				temp+=k;
			}
			end = clock();
			timeMat[i][j][2]=((double) (end - start)) / CLOCKS_PER_SEC;
			//read 1 byte at a time;
			temp = my_arr;
			start = clock();
			for(int i=0;i<N;i++)
			{
				memcpy(&x,temp,1*sizeof(char));
				temp+=1;
			}
			end = clock();
			timeMat[i][j][3]=((double) (end - start)) / CLOCKS_PER_SEC;
			free(my_arr);
		}
	}

	FILE *fptr;
	fptr = fopen("performanceMatrix.txt","w");
	for(int i =0;i<8;i++)
	{
		for(int j =0;j<6;j++)
		{
			for(int k=0;k<4;k++)
			{
				fprintf(fptr,"%lf\t",timeMat[i][j][k]);
			}
		}
	}
	return 0;
}

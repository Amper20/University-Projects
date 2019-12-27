#include<string.h>
#include<stdio.h>
#include<time.h>
#include <pthread.h>
#include <math.h>

int n1GB = 1024*1024*1024;
int  n = 32;
const int P = 8;

char v[1024*1024*1024];

void *fun(void *ID){
	int id = *(int *)ID;
	int start = id*ceil(n/P);
	int end = (id+1)*ceil(n/P);
	printf("%d %d\n", start, end);
	{
		int c = 0;

		for(int i = start;i<end;i++)
			memset(v+(i*32*1024*1024), '1', 32*1024*1024);
		printf("%d \n", c);
	}
}


int main(int argc, char * argv[]) {
	int c;
	memset(v, '1', n1GB);

	clock_t begin = clock();

	int i;
	for(i=0;i<32;i++)
		memset(v+(i*32*1024*1024), '1', 32*1024*1024);


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n",time_spent);
	pthread_t thread[P];
	int id[P];
	for(int i= 0; i < P; i++){
		id[i] = i;
		pthread_create(&thread[i], NULL, fun, &id[i]);
	}
	for(int i= 0; i < P; i++){
		id[i] = i;
		pthread_join(thread[i], NULL);
	}

	return 0;
}
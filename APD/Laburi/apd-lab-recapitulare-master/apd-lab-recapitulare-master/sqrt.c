#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include <pthread.h>

int n = 1024*1024*100;
const int P = 8;
int a = 64;

void *fun(void *ID){
	int id = *(int *)ID;
	int start = id*ceil(n/P);
	int end = (id+1)*ceil(n/P);
	printf("%d %d\n", start, end);
	{
		int c = 0;
		for(int i=start;i<end;i++)
			c += sqrt(a);
		printf("%d \n", c);
	}
}

int main(int argc, char * argv[]) {
	int i, j;
	printf("simple\n");
	clock_t begin = clock();

	{
		int c = 0;
		for(i=0;i<n;i++)
			c += sqrt(a);
		printf("%d \n", c);
	}


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f\n",time_spent);
	printf("on sterids\n");
	
	clock_t b1 = clock();
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
	clock_t e1 = clock();
	time_spent = (double)(e1 - b1) / CLOCKS_PER_SEC;

	
	return 0;
}
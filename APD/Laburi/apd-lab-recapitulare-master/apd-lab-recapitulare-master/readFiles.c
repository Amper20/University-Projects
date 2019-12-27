#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include <pthread.h>
/**
 * @author cristian.chilipirea
 * Create files using
 * dd if=/dev/urandom of=1.txt bs=1024 count=102400
 * dd if=/dev/urandom of=2.txt bs=1024 count=102400
 */

FILE *f, *g;
int n = 1024*1024*100, P = 2;
char buf1[104857600];
char buf2[104857600];

void *fun(void *ID){
	int id = *(int *)ID;
	if(id == 0){
		fread(buf1,sizeof(char),n,f);
	}else{
		fread(buf2,sizeof(char),n,g);
	}
}
int main(int argc, char * argv[]) {

	f = fopen("1.txt","rb");
	g = fopen("2.txt","rb");
	clock_t begin = clock();

	fread(buf1,sizeof(char),n,f);
	fread(buf2,sizeof(char),n,g);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

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

	printf("%f\n",time_spent);
	fclose(f);
	fclose(g);
	return 0;
}
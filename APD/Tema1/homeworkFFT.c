#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <pthread.h>

typedef double complex cplx;
int N, numThread, ignore;
cplx *buf, *out;
pthread_mutex_t mutex;

typedef struct{
	int id,n;
	cplx *buf, *out;
	int step;
}param;

void for_function(cplx buf[], cplx out[], int n, int step){
	for (int i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * M_PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
}

void _fft(cplx buf[], cplx out[], int n, int step){
	if (step < n) {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);
		for_function(buf,out,n,step);
	}
}

//cals _fft function in a separate thread
void* thread_fun(void* ptr){

	param val = *(param*)ptr;
	
	_fft(val.buf, val.out, val.n, val.step);

	pthread_exit(NULL);

}
 
void read_in(int *N, char *in_path){
	FILE *file = fopen(in_path,"r");
	ignore = fscanf(file, "%d", N);

	buf = malloc((*N) * sizeof(cplx));
	out = malloc((*N) * sizeof(cplx));
	
	for(int i = 0 ; i < *N; i++){
		double val; 
		ignore = fscanf(file, "%lf", &val);
		buf[i] = val;
		out[i] = buf[i];
	}
	
	fclose(file);
}

void write_out(char *out_path){

	FILE* fil = fopen(out_path, "w");

	fprintf(fil, "%d\n", N);
	for(int i = 0; i < N; i++)
		fprintf(fil, "%lf %lf\n", creal(buf[i]), cimag(buf[i]));

	fclose(fil);

}

int main(int argc, char *argv[]) {	
	
	numThread = atoi(argv[3]);
	read_in(&N, argv[1]);

	//recursion unroling based on number of threads
	if(numThread == 1)
		_fft(buf, out, N, 1);
	
	//unrol for step = 2
	if(numThread == 2){
		
		pthread_t tid[2];
		param val[2];
		
		val[0].buf = out + 1, val[0].out= buf + 1;
		val[0].n = N, val[0].step = 2;
		pthread_create(&(tid[0]),NULL, thread_fun, &(val[0]));
		
		val[1].buf = out, val[1].out= buf;
		val[1].n = N, val[1].step = 2;
		pthread_create(&(tid[1]),NULL, thread_fun, &(val[1]));
		
		//join threads
		for(int i = 0 ; i < 2; i++)
			pthread_join(tid[i],NULL);
		
		//perform missed for loop
		for_function(buf,out,N,1);
	}
	//unrol for step = 4
	if(numThread == 4){
		
		pthread_t tid[4];
		param val[4];
		
		//create 4 threads that will solve problems separately
		
		val[0].buf = buf, val[0].out = out;
		val[0].n = N, val[0].step = 4;
		pthread_create(&(tid[0]),NULL, thread_fun, &(val[0]));
		
		val[1].buf = buf+2, val[1].out = out+2;
		val[1].n = N, val[1].step = 4;
		pthread_create(&(tid[1]),NULL, thread_fun, &(val[1]));
		
		val[2].buf = buf+1, val[2].out = out+1;
		val[2].n = N, val[2].step = 4;
		pthread_create(&(tid[2]),NULL, thread_fun, &(val[2]));
		
		val[3].buf = buf+3, val[3].out = out+3;
		val[3].n = N, val[3].step = 4;
		pthread_create(&(tid[3]),NULL, thread_fun, &(val[3]));
		
		for(int i = 0 ; i < 4; i++)
			pthread_join(tid[i],NULL);

		//perform missed for loop
		for_function(out, buf, N, 2);
		for_function(out + 1, buf + 1, N, 2);
		for_function(buf, out, N, 1);
	}
	write_out(argv[2]);
	return 0;
}
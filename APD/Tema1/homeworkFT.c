#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
	double re,im;
} complex;

//data
int N, numThread, ignore;
double *x;
complex *X;
//

void read_in(int *N, char *in_path){
	FILE *file = fopen(in_path,"r");
	ignore = fscanf(file, "%d", N);
	x = malloc((*N) * sizeof(double));
	for(int i = 0 ; i < *N; i++)
		ignore = fscanf(file, "%lf", &x[i]);
	fclose(file);
}

void* ft(void* ptr){
	//loop unroling
	int id = *(int *) ptr;
	int start = id*ceil(N/numThread);
	int end = (id+1)*ceil(N/numThread);
	end = (end >= N ? N : end);
	for(int k = start; k < end; k++){
		for(int i = 0; i < N; i++){
			X[k].re += x[i]*cos(((double)2*M_PI*k*i)/(double)N);
			X[k].im -= x[i]*sin(((double)2*M_PI*k*i)/(double)N); 
		}
	}
	pthread_exit(NULL);
}

void write_out(char *out_path){

	FILE* file = fopen(out_path, "w");

	fprintf(file, "%d\n", N);
	for(int i = 0; i < N; i++)
		fprintf(file,"%lf %lf\n", X[i].re, X[i].im);

	fclose(file);
}

int main(int argc, char *argv[]) {
	
	numThread = atoi(argv[3]);
	read_in(&N, argv[1]);
	
	pthread_t tid[numThread];
	int id[numThread];
	
	X = malloc(sizeof(complex) * N);
	for(int i = 0; i < N; i++)
		X[i].re = X[i].im = 0;
	
	for(int i = 0 ; i < numThread; i++)
		id[i] = i;
	
	for(int i = 0 ; i < numThread; i++)
		pthread_create(&(tid[i]), NULL, ft, &(id[i]));
	
	for(int i = 0 ; i < numThread; i++)
		pthread_join(tid[i], NULL);

	write_out(argv[2]);

	free(x);
	free(X);
	
	return 0;
}

#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/**
 * @author 
 *
 */

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	if(rank==0) { // This code is run by a single process
		int polynomialSize;
		int n;
		int i;
		int j;
		float x;

		FILE * polFunctionFile = fopen("a1.txt","rt");
		fscanf(polFunctionFile, "%i", &polynomialSize);
		float * a = malloc(sizeof(float)*polynomialSize);
		for(i=0;i<polynomialSize;i++) {
			fscanf(polFunctionFile, "%f", a+i);
		}
		fclose(polFunctionFile);
		int to_send = a[0];
		MPI_Send(&to_send, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        to_send = a[1];
		MPI_Send(&to_send, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        // to_send = a[2];
		// MPI_Send(&to_send, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        
		for(x=0; x<3; x+=1) {
			float sum = 0;
			float xPowI = 1;
			for(i=0;i<polynomialSize;i++) {
				sum += xPowI * a[i];
				xPowI *= x; // more efficient than pow(x,i);
			}
			printf("%d %f \n",rank, sum);
		}
	}
	
		if(rank == 1){
			int a = 0;
			MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("rank, a : %d %d\n", rank, a);
			for(int x=0; x<3; x+=1){
				int to_send = a;
				MPI_Send(&to_send, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
			}
		}
		if(rank == 2){
			int a = 0;
			MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("rank, a : %d %d\n", rank, a);
			for(int x=0; x<3; x+=1){
				int to_recv = 0;
				MPI_Recv(&to_recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				int ans = a * x + to_recv;
				printf("%d %d\n", x, ans);
			}
		}
		// if(rank == 3){
		// 	int a = 0;
		// 	MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// 	printf("rank, a : %d %d\n", rank, a);
		// 	for(int x=0; x<3; x+=1){
		// 		int to_recv = 0;
		// 		MPI_Recv(&to_recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// 		int ans = a * x*x + to_recv;
		// 		printf("%d %d\n", x, ans);
		// 	}
		// }
	
	MPI_Finalize();
	return 0;
}
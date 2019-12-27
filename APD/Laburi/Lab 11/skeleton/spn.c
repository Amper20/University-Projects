#include<mpi.h>
#include<stdio.h>

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 10 ./a.out
 */

int graph[][2] = { { 0, 1 }, {1,2}, {1,3}, {2,3}};


int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);
	int vec[5] = {-1, -1, -1, -1, -1};
	if(rank == 0){
		for(int i = 0; i < 4; i++){
			if(graph[i][0] == rank)
				MPI_Send(vec, 5, MPI_INT, graph[i][1], 1, MPI_COMM_WORLD);
		}
		MPI_Status stat;
		MPI_Recv(vec, 5, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,  &stat);
	}else{
		MPI_Status stat;
		MPI_Recv(vec, 5, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,  &stat);
		int parent = stat.MPI_SOURCE;
		vec[parent] = 0;
		for(int i = 0; i < 4; i++){
			if(graph[i][0] == rank){
				MPI_Send(vec, 5, MPI_INT, graph[i][1], 1, MPI_COMM_WORLD);
			}
		}
		for(int i = 0; i < 4; i++){
			int vec_rec[5];
			if(graph[i][0] == rank)
				MPI_Recv(vec_rec, 5, MPI_INT, graph[i][1], 1, MPI_COMM_WORLD,  &stat);
			for(int i = 0 ; i < 4;i++){
				if(vec_rec[i]!= -1 && vec[i] == -1){
					vec[i] = vec_rec[i];
				}
			}
		}
		MPI_Send(vec, 5, MPI_INT, parent, 1, MPI_COMM_WORLD);
			
	}
	
	printf("rank: ");
	for(int i = 0 ; i < 4; i++){
		printf("%d ", vec[i]);
	}
	printf("\n");
	
	printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}
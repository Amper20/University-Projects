#include<mpi.h>
#include<stdio.h>

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 10 ./a.out
 */

int graph[][2] = { { 0, 1 }, { 0, 4 }, { 0, 5 }, 
                  { 1, 0 }, { 1, 2 }, { 1, 6 }, 
                  { 2, 1 }, { 2, 3 }, { 2, 7 },
                  { 3, 2 }, { 3, 4 }, { 3, 8 },
                  { 4, 0 }, { 4, 3 }, { 4, 9 },
                  { 5, 0 }, { 5, 7 }, { 5, 8 },
                  { 6, 1 }, { 6, 8 }, { 6, 9 },
                  { 7, 2 }, { 7, 5 }, { 7, 9 },
                  { 8, 3 }, { 8, 5 }, { 8, 6 },
                  { 9, 4 }, { 9, 6 }, { 9, 7 } };


int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	int leader = rank;
	int k = 1000;
	while(k--){
		for(int i = 0 ; i < 30; i++){
			if(graph[i][0] == rank){
				MPI_Send(&leader, 1, MPI_INT, graph[i][1], 1, MPI_COMM_WORLD);
			}
		}
		int val;
		MPI_Recv(&val, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,  MPI_STATUS_IGNORE);
		
		if(val > leader)
			leader = val;
	}
	printf("rank %d, leader %d\n", rank, leader);
	
	float val = 0.0;
	
	if(rank == leader){
		val = 1.0;
		
	}
	k = 50000;
	while (k--)
	
	
	for(int i = 0 ; i < 30; i++){
		if(graph[i][0] == rank){
			MPI_Send(&val, 1, MPI_FLOAT, graph[i][1], 2, MPI_COMM_WORLD);
			float val1;
			MPI_Recv(&val1, 1, MPI_FLOAT, graph[i][1], 2, MPI_COMM_WORLD,  MPI_STATUS_IGNORE);
			val += val1;
			val/=2.0;
		}
	}

	
	if(rank == leader)
		printf("\n\n---------------%f-----------\n\n", 1/val);
	MPI_Finalize();
	return 0;
}
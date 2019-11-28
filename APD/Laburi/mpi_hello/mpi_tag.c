/******************************************************************************
* FILE: mpi_hello.c
* DESCRIPTION:
*   MPI tutorial example code: Simple hello world program
* AUTHOR: Blaise Barney
* LAST REVISED: 03/05/10
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[])
{
    int   numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf ("Hello from task %d on %s!\n", rank, hostname);
    if (rank == 0){
        int to_send[100] = {0};
        printf("Sent \n");
        MPI_Send(&to_send, 100, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Send(&to_send, 100, MPI_INT, 1, 2, MPI_COMM_WORLD);
        MPI_Send(&to_send, 100, MPI_INT, 1, 3, MPI_COMM_WORLD);
    }
    if (rank == 1){
        int to_recv[100] = {1,1,1,1,1};
        MPI_Status stat;
        MPI_Recv(&to_recv, 100, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        printf("Received all good %d %d\n", stat.MPI_SOURCE, stat.MPI_TAG);
        MPI_Recv(&to_recv, 100, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        printf("Received all good %d %d\n", stat.MPI_SOURCE, stat.MPI_TAG);
        MPI_Recv(&to_recv, 100, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        printf("Received all good %d %d\n", stat.MPI_SOURCE, stat.MPI_TAG);
    }
    
    MPI_Finalize();

}


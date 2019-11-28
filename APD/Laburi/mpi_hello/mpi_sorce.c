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
    printf ("Hello from task %d\n", rank);
    if (rank < 3 ){
        int to_send = 4;
        printf("Sent \n");
        MPI_Send(&to_send, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
    }
    if (rank == 3){
        int to_recv = 0;
        MPI_Status status;
        MPI_Recv(&to_recv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printf("Received all good %d %d\n", to_recv, status.MPI_SOURCE);

        MPI_Recv(&to_recv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printf("Received all good %d %d\n", to_recv, status.MPI_SOURCE);
        
        MPI_Recv(&to_recv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printf("Received all good %d %d\n", to_recv, status.MPI_SOURCE);
    }
    
    MPI_Finalize();

}


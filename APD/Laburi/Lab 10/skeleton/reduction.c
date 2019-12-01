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
#define MASTER	0

int main (int argc, char *argv[])
{
    int   numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf ("Hello from task %d on %s!\n", rank, hostname);
    
    int to_send = rank, to_recv = 0;

    for(int i = 1; i < numtasks; i*=2){

        if(rank % (2*i) == i){
            MPI_Send(&to_send, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD);
        }else{
            if(rank % (2*i) == 0){
                MPI_Recv(&to_recv, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                to_send += to_recv;
            }
        }

    }
    if(rank == 0)
        printf("----------Final %d----------\n", to_recv);
    
    MPI_Finalize();

}


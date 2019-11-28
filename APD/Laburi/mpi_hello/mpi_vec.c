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
        
        int to_send[99] = {0};
        
        printf("Sent \n");

        for (int i = 0 ; i < 99; i++)
            to_send[i] = i;
        int *pnt = to_send;
        MPI_Send(pnt, 33, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(pnt + 33, 33, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(pnt + 66, 33, MPI_INT, 3, 0, MPI_COMM_WORLD);
        MPI_Recv(pnt, 33, MPI_INT, 1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(pnt + 33, 33, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(pnt + 66, 33, MPI_INT, 3, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i = 0 ; i < 99; i++){
            printf("%d ", to_send[i]);
        }
        
    }
    if (rank == 1){
        int to_recv[33];
        MPI_Recv(&to_recv, 33, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i = 0 ; i < 33; i++)
            to_recv[i] = 42;
        
        MPI_Send(&to_recv, 33, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }
    if (rank == 2){
        int to_recv[33];
        MPI_Recv(&to_recv, 33, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i = 0 ; i < 33; i++)
            to_recv[i] = 42;
        
        MPI_Send(&to_recv, 33, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 3){
        int to_recv[33];
        MPI_Recv(&to_recv, 33, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i = 0 ; i < 33; i++)
            to_recv[i] = 42;
        
        MPI_Send(&to_recv, 33, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    
    MPI_Finalize();

}


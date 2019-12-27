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
int r = 100;
int getA(){
    double ret = 0;
    int x = rand()%r;
    int y = rand()%r;
    if(x*x + y*y <= r*r){
        return 1;
    }
    return 0;
}
int main (int argc, char *argv[])
{
    int   numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf ("Hello from task %d on %s!\n", rank, hostname);
    if (rank == 0){
        int area[2] = {0,0};
        int recv_area[2] = {0,0};
        MPI_Status stat;
        for(int i = 0 ; i  < numtasks-1; i++){
            MPI_Recv(&recv_area, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
            area[0] += recv_area[0];
            area[1] += recv_area[1];
        }
        printf("--------%lf---------\n",4*(double)area[0]/(double)area[1]);
    }else{
        int area[2] = {0,0};
        for(int i = 0 ;i  < 100000000/numtasks; i++){
            if(getA())
                area[0] += 1;
            area[1] += 1;

        }
        MPI_Send(&area, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    
    MPI_Finalize();

}


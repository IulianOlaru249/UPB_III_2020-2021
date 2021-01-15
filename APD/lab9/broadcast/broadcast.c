#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int value;
    int recv_value = 0;
    MPI_Status status;


    if (rank == MASTER) {
        value = 7;
    }

    for (int i = 1; i < procs; i *= 2) {
        // TODO
        if (rank < i && (rank + i) < procs) {
            MPI_Send(&value, 1, MPI_INT, (rank + i), 0, MPI_COMM_WORLD);
            printf("step %d -> %d sends %d, having the value %d\n", i, rank, (rank + i), value);
            
        } else if (rank >= i && rank < i * 2) {
            MPI_Recv(&recv_value, 1, MPI_INT, (rank - i), 0, MPI_COMM_WORLD, &status);
            value = recv_value;
            printf("step %d -> %d recieves from %d, having the value %d\n", i, rank, (rank - i), value);
        }

    }

    printf("Process [%d] has value = %d\n", rank, value);

    MPI_Finalize();

}


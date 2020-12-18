#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Checks the number of processes allowed.
    if (numtasks != 2) {
        printf("Wrong number of processes. Only 2 allowed!\n");
        MPI_Finalize();
        return 0;
    }

    // How many numbers will be sent.
    srand(time(NULL));
    int send_numbers = 10;

    if (rank == ROOT) {
        // Sends the numbers with the tags to the second process.
        for (int i = 0; i < send_numbers; i++) {
            // Generate the random numbers.
            int rand_value = rand() % (rank * 50 + 1) + i;
            // Generate the random tags.
            int rand_tag = rand() % (rank * 50 + 1) + i;
            MPI_Send(&rand_value, 1, MPI_INT, 1, rand_tag, MPI_COMM_WORLD);
            printf("Process with rank %d, sent %d with tag %d\n",
                rank, rand_value, rand_tag);
        }
    } else {

        // Receives the information from the first process.
        MPI_Status status;
        for (int i = 0; i < send_numbers; i++) {
            int recv_value;
            MPI_Recv(&recv_value, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // Prints the numbers with their corresponding tags.
            printf("Process with rank %d, received %d with tag %d\n",
                rank, recv_value, status.MPI_TAG);
        }
    }

    MPI_Finalize();

}


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LIMIT 10

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            fprintf(stderr, "This program requires exactly 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int count = 0;
    int partner = 1 - rank;
    while (count < LIMIT) {
        if (count % 2 == rank) {
            count++;
            MPI_Send(&count, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            printf("Rank %d sent count %d to rank %d\n", rank, count, partner);
        } else {
            MPI_Recv(&count, 1, MPI_INT, partner, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            printf("Rank %d received count %d from rank %d\n",
                   rank, count, partner);
        }
    }

    MPI_Finalize();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int left  = (rank - 1 + size) % size;
    int right = (rank + 1) % size;

    int from_left = -1;
    MPI_Request request;

    double t0 = MPI_Wtime();

    MPI_Isend(&rank, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &request);
    MPI_Recv(&from_left, 1, MPI_INT, left, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Wait(&request, MPI_STATUS_IGNORE);

    printf("Rank %d received value %d from rank %d\n",
           rank, from_left, left);

    if (rank == 0)
        printf("Communication time: %.6f s\n", MPI_Wtime() - t0);

    MPI_Finalize();
    return 0;
}

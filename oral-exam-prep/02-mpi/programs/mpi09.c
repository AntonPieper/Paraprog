#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LOCAL 10

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int left  = (rank - 1 + size) % size;
    int right = (rank + 1) % size;

    double local[LOCAL];
    for (int i = 0; i < LOCAL; i++)
        local[i] = (double)rank;

    double left_halo = -1.0, right_halo = -1.0;

    MPI_Sendrecv(&local[LOCAL - 1], 1, MPI_DOUBLE, right, 0,
                 &left_halo,        1, MPI_DOUBLE, left,  0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    MPI_Sendrecv(&local[0],   1, MPI_DOUBLE, left,  1,
                 &right_halo, 1, MPI_DOUBLE, right, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Rank %d: left_halo = %.0f, right_halo = %.0f\n",
           rank, left_halo, right_halo);

    MPI_Finalize();
    return 0;
}

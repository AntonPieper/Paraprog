#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4   /* elements per process */

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local[N];
    for (int i = 0; i < N; i++)
        local[i] = rank * N + i;

    int *all = NULL;
    if (rank == 0)
        all = malloc((size_t)size * N * sizeof(int));

    MPI_Gather(local, N, MPI_INT,
               all, N * size, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered: ");
        for (int i = 0; i < size * N; i++)
            printf("%d ", all[i]);
        printf("\n");
        free(all);
    }

    MPI_Finalize();
    return 0;
}

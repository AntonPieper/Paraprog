#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000

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

    int *sendbuf = malloc(N * sizeof(int));
    int *recvbuf = malloc(N * sizeof(int));
    for (int i = 0; i < N; i++)
        sendbuf[i] = rank * N + i;

    int partner = 1 - rank;
    MPI_Ssend(sendbuf, N, MPI_INT, partner, 0, MPI_COMM_WORLD);
    MPI_Recv(recvbuf, N, MPI_INT, partner, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    printf("Rank %d: exchange complete, recvbuf[0] = %d\n",
           rank, recvbuf[0]);

    free(sendbuf);
    free(recvbuf);
    MPI_Finalize();
    return 0;
}

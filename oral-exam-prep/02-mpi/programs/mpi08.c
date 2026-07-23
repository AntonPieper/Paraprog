#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100000

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

    int *buf = malloc(N * sizeof(int));

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            buf[i] = i;

        MPI_Request request;
        MPI_Isend(buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        /* prepare next iteration while the message is in flight */
        for (int i = 0; i < N; i++)
            buf[i] = -1;

        MPI_Wait(&request, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(buf, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1: buf[0] = %d, buf[N-1] = %d (expected: 0 and %d)\n",
               buf[0], buf[N - 1], N - 1);
    }

    free(buf);
    MPI_Finalize();
    return 0;
}

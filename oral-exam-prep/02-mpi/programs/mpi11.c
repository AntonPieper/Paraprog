#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long n = (argc > 1) ? atol(argv[1]) : 100000000L;
    long chunk = n / size;
    double *data = NULL;

    double t0 = MPI_Wtime();

    if (rank == 0) {
        data = malloc(n * sizeof(double));
        for (long i = 0; i < n; i++)
            data[i] = 1.0;

        /* distribute one chunk to each worker, then collect results */
        double total = 0.0;
        for (int w = 1; w < size; w++)
            MPI_Send(&data[(long)w * chunk], chunk, MPI_DOUBLE, w, 0,
                     MPI_COMM_WORLD);

        for (long i = 0; i < chunk; i++)
            total += data[i];

        for (int w = 1; w < size; w++) {
            double partial;
            MPI_Recv(&partial, 1, MPI_DOUBLE, w, 1, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            total += partial;
        }
        printf("total = %.1f, time = %.3f s\n", total, MPI_Wtime() - t0);
        free(data);
    } else {
        double *mydata = malloc(chunk * sizeof(double));
        MPI_Recv(mydata, chunk, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        double partial = 0.0;
        for (long i = 0; i < chunk; i++)
            partial += mydata[i];
        MPI_Send(&partial, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        free(mydata);
    }

    MPI_Finalize();
    return 0;
}

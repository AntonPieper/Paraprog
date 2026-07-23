#include <stdio.h>
#include <unistd.h>
#include <omp.h>

static void process(int item) {
    usleep(1000);
    printf("item %d processed by thread %d\n", item, omp_get_thread_num());
}

int main(void) {
    int i;
    #pragma omp parallel num_threads(4)
    #pragma omp single
    {
        for (i = 0; i < 8; i++) {
            #pragma omp task
            process(i);
        }
    }
    return 0;
}

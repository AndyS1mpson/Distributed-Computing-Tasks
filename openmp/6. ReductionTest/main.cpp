#include <omp.h>
#include <stdio.h>
#include <time.h>

#define ITERATION_COUNT 1000000
#define CHUNK_SIZE 10000

struct ReductionResult {

    ReductionResult(int s, double execTime) {
        sum = s;
        executionTime = execTime;    
    };

    int sum;
    double executionTime;
};


ReductionResult runReduction() {
    int sum = 0;
    double startTime = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, CHUNK_SIZE) reduction(+: sum)
    for (int i = 0; i < ITERATION_COUNT; ++i) {
        sum += 1;
    }
    double endTime = omp_get_wtime();
    return ReductionResult(sum, endTime - startTime);
}

ReductionResult runReductionWithAtomic() {
    int sum = 0;
    double startTime = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, CHUNK_SIZE) reduction(+: sum)
    for (int i = 0; i < ITERATION_COUNT; ++i) {
        #pragma omp atomic
        sum += 1;
    }
    double endTime = omp_get_wtime();
    return ReductionResult(sum, endTime - startTime);
}

ReductionResult runReductionWithCritical() {
    double sum = 0;

    double startTime = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, CHUNK_SIZE) reduction(+: sum)
    for (int i = 0; i < ITERATION_COUNT; ++i) {
        #pragma omp critical
        sum += 1;
    }
    double endTime = omp_get_wtime();
    return ReductionResult(sum, endTime - startTime);
}

ReductionResult runReductionWithLock() {
    double sum = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);
    double startTime = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, CHUNK_SIZE) reduction(+: sum)
    for (int i = 0; i < ITERATION_COUNT; ++i) {
        omp_set_lock(&lock);
        sum += 1;
        omp_unset_lock(&lock);
    }
    double endTime = omp_get_wtime();
    return ReductionResult(sum, endTime - startTime);
}

int main()
{
    ReductionResult res1 = runReduction();
    ReductionResult res2 = runReductionWithAtomic();
    ReductionResult res3 = runReductionWithCritical();
    ReductionResult res4 = runReductionWithLock();

    printf("Sum = %d; Reduction spent time = %f\n", res1.sum, res1.executionTime);
    printf("Sum = %d; Atomic spent time = %f\n", res2.sum, res2.executionTime);
    printf("Sum = %d; Critical spent time = %f\n", res3.sum, res3.executionTime);
    printf("Sum = %d; Lock spent time = %f\n", res4.sum, res4.executionTime);

}

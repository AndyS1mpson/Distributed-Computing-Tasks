/*
Задача:
Найти максимальный(минимальный) элемент массива.
*/
#include <iostream>
#include <stdio.h> 
#include <unistd.h> 
#include <ctime>
#include <mpi.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX_RAND_VAL 100000   // верхняя граница значений вектора
#define VECTOR_SIZE 100000000     // размер вектора
#define ROOT 0                // главный поток
#define LOCAL_BUF_SIZE 64

int* generateVector(int size) {
    int* vec = new int[size];
    srand(time(NULL));
    for (int i=0; i<size; i++) {
        vec[i] = rand() % MAX_RAND_VAL + 1;
    }
    return vec;
}

int findMin(int* vec, int size) {
    int min = INT32_MAX;

    for (int i = 0; i < size; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}

int findMinMPI(int* vec, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* recBuf = new int[n / size];
    int resBuf[LOCAL_BUF_SIZE];
    int localMin = INT32_MAX;
    
    MPI_Scatter(vec, n / size, MPI_INT, recBuf, n / size, MPI_INT, 0, MPI_COMM_WORLD);
    for(int i = 0; i < n / size; i++){
        if(localMin >= recBuf[i]){
            localMin = recBuf[i];
        }
    }
    MPI_Gather(&localMin, 1, MPI_INT, resBuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == ROOT) {
        return findMin(resBuf, size);
    }
}

long testSEQ(int vectorSize) {
    int* vec = generateVector(vectorSize);
    auto start = MPI_Wtime();
    findMin(vec, vectorSize);
    auto end = MPI_Wtime();
    return end - start;
}

double testMPI(int vectorSize) {
    int rank;
    int* vec = generateVector(vectorSize);
    double start, end;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == ROOT){
        start = MPI_Wtime();
    }
    int result = findMinMPI(vec, vectorSize);
    if(rank == ROOT){
        end = MPI_Wtime();
    }
    return end - start;
}


int main(int args, char** argv) {


    MPI_Init(&args, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    long seqAlgExecTime = testSEQ(VECTOR_SIZE);
    double mpiAlgExecTime = testMPI(VECTOR_SIZE);
    
    if(rank == ROOT) {
        cout << "Vector size: " << VECTOR_SIZE << endl;
        cout << "Sequential algorithm execution time: " << seqAlgExecTime << endl;
        cout << "MPI algorithm execution time: " << mpiAlgExecTime << endl;
    }
    MPI_Finalize();    
    return 0;
}

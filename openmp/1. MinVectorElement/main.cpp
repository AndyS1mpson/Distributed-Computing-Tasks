/*
Задача:
Найти максимальный(минимальный) элемент массива.
*/
#include <iostream>
#include <omp.h>
#include <vector>
#include <stdio.h>
#include <fstream>
using namespace std;

#define MAX_RAND_VAL 1000000 // верхняя граница значений вектора
#define MAX_THREADS 15       // больше числа ядер
#define MIN_THREADS 6        // меньше числа ядер
#define CORES_NUM 10         // равно числу ядер (Apple M1 Pro)
#define MAX_ARRAY_SIZE 50000 // максимальный размер генерируемого вектора
#define MIN_ARRAY_SIZE 5000  // минимальный размер генерируемого вектора
#define ITER_STEP 5000       // шаг изменения размера массива

struct TestResult{
    int generatedArraySize;
    double searchExecutionTime;
};


struct ThreadsNumTest {
  TestResult* results;
  int size;
};

void saveTestResultsToFile(ThreadsNumTest results, string filename) {
    ofstream file;
    file.open(filename);
    for (int i = 0; i < results.size; i++) {
      file << results.results[i].generatedArraySize << ", " << results.results[i].searchExecutionTime * 1000 << endl;
    }
    file.close();
}


int* generateVector(int size) {
    int* s = new int[size];
    srand(time(NULL));
    for (int i=0; i<size; i++) {
      s[i] = rand() % MAX_RAND_VAL + 1;
    };
    return s;
}


int findMin(int* vec, int n, int threads_num) {
    int min = INT32_MAX;
    #pragma omp parallel for reduction(min:min) num_threads(threads_num)
    for (int i = 0; i < n; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}


ThreadsNumTest test(int threads_num) {
    int cur_vec_size = MIN_ARRAY_SIZE;
    int iter_count = (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE) / ITER_STEP;            // количество итераций/изменений размера массива
    TestResult* results = new TestResult[iter_count];                          // результаты в виде <размер массива, время работы>
    for (int i = 0; i < iter_count; i++) {
        int* vec = generateVector(cur_vec_size);

        double start = omp_get_wtime();
        findMin(vec, cur_vec_size, threads_num);
        double end = omp_get_wtime();

        delete[] vec;

        results[i].generatedArraySize = cur_vec_size;
        results[i].searchExecutionTime = end - start;
        cur_vec_size = cur_vec_size + ITER_STEP;
    }

    struct ThreadsNumTest res;
    res.results = results;
    res.size = iter_count;

    return res;
}


int main(int argc, char *argv[]) {
    ThreadsNumTest results_1 = test(MAX_THREADS);
    saveTestResultsToFile(results_1, "test_with_15_threads.txt");

    ThreadsNumTest results_2 = test(MIN_THREADS);
    saveTestResultsToFile(results_2, "test_with_6_threads.txt");
  
    ThreadsNumTest results_3 = test(CORES_NUM);
    saveTestResultsToFile(results_3, "test_with_10_threads.txt");
}

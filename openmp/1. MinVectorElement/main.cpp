/*
Задача:
Найти максимальный(минимальный) элемент массива.
*/
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX_RAND_VAL 100000    // верхняя граница значений вектора

#define MAX_THREADS 30         // больше числа ядер
#define MIN_THREADS 4          // меньше числа ядер
#define THREAD_STEP 2          // шаг изменения числа потоков
#define CORES_NUM 10           // равно числу ядер (Apple M1 Pro)

#define MAX_ARRAY_SIZE 1000000 // максимальный размер генерируемого вектора
#define MIN_ARRAY_SIZE 10000   // минимальный размер генерируемого вектора
#define ITER_STEP 20000        // шаг изменения размера вектора

#define RETRIES 10


// Структура описывающая результат теста
// поиска минимального элемента вектора
struct TestResult {

    TestResult(int p, double execTime) {
        parameter = p;
        executionTime = execTime;    
    };

    int parameter;
    double executionTime;
};


void saveTestResultsToFile(list<TestResult> testRestults, string filename) {
    ofstream file;
    file.open(filename);
    for (TestResult res : testRestults) {
      file << res.parameter << " " << res.executionTime<< endl;
    }
    file.close();
}


int* generateVector(int size) {
    int* vec = new int[size];
    srand(time(NULL));
    for (int i=0; i<size; i++) {
      vec[i] = rand() % MAX_RAND_VAL + 1;
    };
    return vec;
}

// Последовательная версия
int findMin(int* vec, int size) {
    int min = INT32_MAX;
    for (int i = 0; i < size; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}

// Параллельная версия
int findMinOMP(int* vec, int size, int threadsNum) {
    int min = INT32_MAX;
    #pragma omp parallel for reduction(min:min) num_threads(threadsNum)
    for (int i = 0; i < size; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}


long testPerfDependsOnTheNumOfThreads(int vectorSize) {
    int* vec = generateVector(vectorSize);
    auto start = high_resolution_clock::now();
    findMin(vec, vectorSize);
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();        
}


list<TestResult> testPerfDependsOnTheNumOfThreadsOMP(int vectorSize) {
    int iters = (MAX_THREADS - MIN_THREADS) / THREAD_STEP;
    int curThreadsNum = MIN_THREADS;
    list<TestResult> results;
    for (int i = 0; i <= iters; i++) {
        cout << "Current threads num: " << curThreadsNum << endl;
        int* vec = generateVector(vectorSize);
        
        double time = 0.0;
        for (int j = 0; j < RETRIES; j++) {
            double start = omp_get_wtime();
            findMinOMP(vec, vectorSize, curThreadsNum);
            double end = omp_get_wtime();
            time += end - start;
        }
        results.push_back(TestResult(curThreadsNum, (time / RETRIES) * 1000000));
        curThreadsNum += THREAD_STEP;
    }
    return results;
}


list<TestResult> test(int threadsNum) {
    int curVecSize = MIN_ARRAY_SIZE;
    int numOfIters = (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE) / ITER_STEP;     // количество итераций/изменений размера массива
    list<TestResult> results;                                           // результаты в виде <размер массива, время работы>
    for (int i = 0; i <= numOfIters; i++) {
        int* vec = generateVector(curVecSize);

        double start = omp_get_wtime();
        findMinOMP(vec, curVecSize, threadsNum);
        double end = omp_get_wtime();

        results.push_back(TestResult(curVecSize, (end - start)));
        curVecSize = curVecSize + ITER_STEP;
    }

    return results;
}


int main(int argc, char *argv[]) {

    // Thread tests
    cout << "Sequential algorithm time execution: " << testPerfDependsOnTheNumOfThreads(MAX_ARRAY_SIZE) << endl;
    list<TestResult> tres1 = testPerfDependsOnTheNumOfThreadsOMP(MAX_ARRAY_SIZE);
    saveTestResultsToFile(tres1, "thread_test.txt");

    // list<TestResult> res1 = test(MAX_THREADS);
    // saveTestResultsToFile(res1, "test_with_15_threads.txt");

    // list<TestResult> res2 = test(MIN_THREADS);
    // saveTestResultsToFile(res2, "test_with_5_threads.txt");
  
    // list<TestResult> res3 = test(CORES_NUM);
    // saveTestResultsToFile(res3, "test_with_10_threads.txt");
}

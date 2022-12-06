/*
Задача:
Реализовать поиск максимального из минимальных элементов строк треугольной матрицы.
*/
#include <iostream>
#include <math.h> 
#include <omp.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX_MATRIX_SIZE 10000   // максимальный размер генерируемого вектора
#define MIN_MATRIX_SIZE 100     // минимальный размер генерируемого вектора
#define ITER_STEP 100           // шаг изменения размера вектора
#define MAX_RAND_VAL 1000000    // верхняя граница значений матрицы

#define MAX_THREADS 30          // больше числа ядер
#define MIN_THREADS 4           // меньше числа ядер
#define THREAD_STEP 2           // шаг изменения числа потоков
#define CORES_NUM 10            // равно числу ядер (Apple M1 Pro)

#define RETRIES 10

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

int** generateMatrix(int n, int m) {
    int** matrix = new int* [n];
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[m];
        for (int j = 0; j <= i; j++){
            matrix[i][j] = rand() % MAX_RAND_VAL + 1;
        }
    };
    return matrix;
}

int findMinVectorElement(int* vec, int n)
{
    int min = vec[0];
    for (int i = 1; i < n; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}

int findMaxMinValue(int** matrix, int n, int m) {
    int maxMin = INT32_MIN;
    for (int i = 0; i < n; i++) {
        int localMin = findMinVectorElement(matrix[i], i + 1);
        if (localMin > maxMin) {
            maxMin = localMin;
        }
    }
    return maxMin;
}

int findMaxMinValueOMP(int** matrix, int n, int m, int threadsNum) {
    int maxMin = INT32_MIN;
    #pragma omp parallel for num_threads(threadsNum)
    for (int i = 0; i < n; i++) {
        int localMin = findMinVectorElement(matrix[i], i + 1);
        #pragma omp critical
        if (localMin > maxMin) {
            maxMin = localMin;
        }
    }
    return maxMin;
}


long testPerfDependsOnTheNumOfThreads(int n, int m) {
    int** matrix = generateMatrix(n, m);
    auto start = high_resolution_clock::now();
    findMaxMinValue(matrix, n, m);
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start).count();        
}


list<TestResult> testPerfDependsOnTheNumOfThreadsOMP(int n, int m) {
    int iters = (MAX_THREADS - MIN_THREADS) / THREAD_STEP;
    int curThreadsNum = MIN_THREADS;
    list<TestResult> results;
    for (int i = 0; i <= iters; i++) {
        cout << "Current threads num: " << curThreadsNum << endl;
        int** matrix = generateMatrix(n, m);
        
        double time = 0.0;
        for (int j = 0; j < RETRIES; j++) {
            double start = omp_get_wtime();
            findMaxMinValueOMP(matrix, n, m, curThreadsNum);
            double end = omp_get_wtime();
            time += end - start;
        }
        results.push_back(TestResult(curThreadsNum, (time / RETRIES) * 1000));
        curThreadsNum += THREAD_STEP;
    }
    return results;
}


list<TestResult> test(int threadsNum) {
    int matrixSize = MIN_MATRIX_SIZE;
    int numOfIters = (MAX_MATRIX_SIZE - MIN_MATRIX_SIZE) / ITER_STEP;     // количество итераций/изменений размера массива
    list<TestResult> results;                                           // результаты в виде <размер массива, время работы>
    for (int i = 0; i <= numOfIters; i++) {
        int** matrix = generateMatrix(matrixSize, matrixSize);

        double start = omp_get_wtime();
        findMaxMinValueOMP(matrix, matrixSize, matrixSize, threadsNum);
        double end = omp_get_wtime();

        results.push_back(TestResult(matrixSize, (end - start)));
        matrixSize = matrixSize + ITER_STEP;
    }

    return results;
}

int main()
{
    // Thread tests
    cout << "Sequential algorithm time execution: " << testPerfDependsOnTheNumOfThreads(MAX_MATRIX_SIZE, MAX_MATRIX_SIZE) << endl;
    list<TestResult> tres1 = testPerfDependsOnTheNumOfThreadsOMP(MAX_MATRIX_SIZE, MAX_MATRIX_SIZE);
    saveTestResultsToFile(tres1, "thread_test.txt");

    // list<TestResult> res1 = test(MAX_THREADS);
    // saveTestResultsToFile(res1, "test_with_15_threads.txt");

    // list<TestResult> res2 = test(MIN_THREADS);
    // saveTestResultsToFile(res2, "test_with_5_threads.txt");
  
    // list<TestResult> res3 = test(CORES_NUM);
    // saveTestResultsToFile(res3, "test_with_10_threads.txt");
}

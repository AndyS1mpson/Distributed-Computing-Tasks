#include <iostream>
#include <math.h> 
#include <omp.h>
#include <stdio.h>
#include <list>
#include <fstream>
using namespace std;

#define MAX_MATRIX_SIZE 1000 // максимальный размер генерируемого вектора
#define MIN_MATRIX_SIZE 100 // минимальный размер генерируемого вектора
#define ITER_STEP 100       // шаг изменения размера вектора
#define MAX_RAND_VAL 10000    // верхняя граница значений матрицы
#define MAX_THREADS 15        // больше числа ядер
#define MIN_THREADS 5         // меньше числа ядер
#define CORES_NUM 10          // равно числу ядер (Apple M1 Pro)

// Структура описывающая тест поиска минимального элемента
// при определенном размере массива
struct TestResult {

    TestResult(int size, double execTime) {
        generatedVectorSize = size;
        executionTime = execTime;    
    };

    int generatedVectorSize;
    double executionTime;
};

void saveTestResultsToFile(list<TestResult> testRestults, string filename) {
    ofstream file;
    file.open(filename);
    for (TestResult res : testRestults) {
      file << res.generatedVectorSize << " " << res.executionTime<< endl;
    }
    file.close();
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

int** generateMatrix(int n, int m) {
    int** matrix = new int* [n];
    srand(time(NULL));
    for (int i=0; i<n; i++) {
        matrix[i] = new int[m];
        for (int j = 0; j < m; j++){
            matrix[i][j] = rand() % MAX_RAND_VAL + 1;
        }
    };
    return matrix;
}

int findMaxMinValue(int** matrix, int n, int m, int threadsNum) {
    int maxMin = INT32_MIN;
    #pragma omp parallel for num_threads(threadsNum)
    for (int i = 0; i < n; i++) {
        int localMin = findMinVectorElement(matrix[i], m);
        #pragma omp critical
        if (localMin > maxMin) {
            maxMin = localMin;
        }
    }
    return maxMin;
}


list<TestResult> test(int threadsNum) {
    int matrixSize = MIN_MATRIX_SIZE;
    int numOfIters = (MAX_MATRIX_SIZE - MIN_MATRIX_SIZE) / ITER_STEP;     // количество итераций/изменений размера массива
    list<TestResult> results;                                           // результаты в виде <размер массива, время работы>
    for (int i = 0; i <= numOfIters; i++) {
        int** matrix = generateMatrix(matrixSize, matrixSize);

        double start = omp_get_wtime();
        findMaxMinValue(matrix, matrixSize, matrixSize, threadsNum);
        double end = omp_get_wtime();

        results.push_back(TestResult(matrixSize, (end - start)));
        matrixSize = matrixSize + ITER_STEP;
    }

    return results;
}

int main()
{
    list<TestResult> res1 = test(MAX_THREADS);
    saveTestResultsToFile(res1, "test_with_15_threads.txt");

    list<TestResult> res2 = test(MIN_THREADS);
    saveTestResultsToFile(res2, "test_with_5_threads.txt");
  
    list<TestResult> res3 = test(CORES_NUM);
    saveTestResultsToFile(res3, "test_with_10_threads.txt");
}

/*
Задача:
Найти максимальный(минимальный) элемент массива.
*/
#include <iostream>
#include <omp.h>
#include <vector>
#include <stdio.h>
#include <list>
#include <fstream>
using namespace std;

#define MAX_RAND_VAL 100000   // верхняя граница значений вектора
#define MAX_THREADS 15       // больше числа ядер
#define MIN_THREADS 5        // меньше числа ядер
#define CORES_NUM 10         // равно числу ядер (Apple M1 Pro)
#define MAX_ARRAY_SIZE 200000 // максимальный размер генерируемого вектора
#define MIN_ARRAY_SIZE 100000  // минимальный размер генерируемого вектора
#define ITER_STEP 20000       // шаг изменения размера массива


// Структура описывающая тест поиска минимального элемента
// при определенном размере массива
struct TestResult {

    TestResult(int size, double execTime) {
        generatedArraySize = size;
        searchExecutionTime = execTime;    
    };

    int generatedArraySize;
    double searchExecutionTime;
};


void saveTestResultsToFile(list<TestResult> testRestults, string filename) {
    ofstream file;
    file.open(filename);
    for (TestResult res : testRestults) {
      file << res.generatedArraySize << " " << res.searchExecutionTime<< endl;
    }
    file.close();
}


vector<int> generateVector(int size) {
    vector<int> vec;
    srand(time(NULL));
    for (int i=0; i<size; i++) {
      vec.push_back(rand() % MAX_RAND_VAL + 1);
    };
    return vec;
}


int findMin(vector<int> vec, int threadsNum) {
    int min = INT32_MAX;
    #pragma omp parallel for reduction(min:min) num_threads(threadsNum)
    for (int i = 0; i < vec.size(); i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}


list<TestResult> test(int threadsNum) {
    int curVecSize = MIN_ARRAY_SIZE;
    int numOfIters = (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE) / ITER_STEP;     // количество итераций/изменений размера массива
    list<TestResult> results;                                           // результаты в виде <размер массива, время работы>
    for (int i = 0; i <= numOfIters; i++) {
        vector<int> vec = generateVector(curVecSize);

        double start = omp_get_wtime();
        findMin(vec, threadsNum);
        double end = omp_get_wtime();

        results.push_back(TestResult(curVecSize, (end - start)));
        curVecSize = curVecSize + ITER_STEP;
    }

    return results;
}


int main(int argc, char *argv[]) {
    list<TestResult> results_1 = test(MAX_THREADS);
    saveTestResultsToFile(results_1, "test_with_15_threads.txt");

    list<TestResult> results_2 = test(MIN_THREADS);
    saveTestResultsToFile(results_2, "test_with_5_threads.txt");
  
    list<TestResult> results_3 = test(CORES_NUM);
    saveTestResultsToFile(results_3, "test_with_10_threads.txt");
}

/*
Задача:
Найти скалярное произведение векторов.
*/
#include <iostream>
#include <omp.h>
#include <vector>
#include <list>
#include <fstream>
using namespace std;

#define MAX_RAND_VAL 100000    // верхняя граница значений вектора
#define MAX_THREADS 15         // больше числа ядер
#define MIN_THREADS 5          // меньше числа ядер
#define CORES_NUM 10           // равно числу ядер (Apple M1 Pro)
#define MAX_VECTOR_SIZE 200000  // максимальный размер генерируемого вектора
#define MIN_VECTOR_SIZE 100000  // минимальный размер генерируемого вектора
#define ITER_STEP 20000        // шаг изменения размера вектора


// Структура описывающая тест
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

vector<int> generateVector(int size) {
    vector<int> vec;
    srand(time(NULL));
    for (int i=0; i<size; i++) {
      vec.push_back(rand() % MAX_RAND_VAL + 1);
    };
    return vec;
}


int dotProduct(vector<int> a, vector<int> b) {
    if (a.size() != b.size()) {
        throw invalid_argument("vectors must have the same size");
    }
    int result = 0;
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    };
    return result;
}


list<TestResult> test(int threadsNum) {
    int curVecSize = MIN_VECTOR_SIZE;
    int numOfIters = (MAX_VECTOR_SIZE - MIN_VECTOR_SIZE) / ITER_STEP;
    list<TestResult> results;       // результаты в виде <размер массива, время работы>
    for (int i = 0; i <= numOfIters; i++) {
        vector<int> a = generateVector(curVecSize);
        vector<int> b = generateVector(curVecSize);

        double start = omp_get_wtime();
        dotProduct(a, b);
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

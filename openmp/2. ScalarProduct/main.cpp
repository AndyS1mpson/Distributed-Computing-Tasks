/*
Задача:
Найти скалярное произведение векторов.
*/
#include <iostream>
#include <omp.h>
#include <list>
#include <chrono>
#include <fstream>
using namespace std;
using namespace std::chrono;


#define MAX_RAND_VAL 100000    // верхняя граница значений вектора

#define MAX_THREADS 30         // больше числа ядер
#define MIN_THREADS 4          // меньше числа ядер
#define CORES_NUM 10           // равно числу ядер (Apple M1 Pro)
#define THREAD_STEP 2

#define MAX_VECTOR_SIZE 1000000// максимальный размер генерируемого вектора
#define MIN_VECTOR_SIZE 10000  // минимальный размер генерируемого вектора
#define ITER_STEP 10000        // шаг изменения размера вектора

#define RETRIES 10

// Структура описывающая тест
// при определенном размере массива
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

int dotProduct(int* a, int* b, int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result += a[i] * b[i];
    };
    return result;
}

int dotProductOMP(int* a, int* b, int size, int threadsNum) {
    int result = 0;
    #pragma omp parallel for reduction(+:result) num_threads(threadsNum)
    for (int i = 0; i < size; i++) {
        result += a[i] * b[i];
    };
    return result;
}

long testPerfDependsOnTheNumOfThreads(int vectorSize) {
    int* a = generateVector(vectorSize);
    int* b = generateVector(vectorSize);
    auto start = high_resolution_clock::now();
    dotProduct(a, b, vectorSize);
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();        
}


list<TestResult> testPerfDependsOnTheNumOfThreadsOMP(int vectorSize) {
    int iters = (MAX_THREADS - MIN_THREADS) / THREAD_STEP;
    int curThreadsNum = MIN_THREADS;
    list<TestResult> results;
    for (int i = 0; i <= iters; i++) {
        cout << "Current threads num: " << curThreadsNum << endl;
        int* a = generateVector(vectorSize);
        int* b = generateVector(vectorSize);
        
        double time = 0.0;
        for (int j = 0; j < RETRIES; j++) {
            double start = omp_get_wtime();
            dotProductOMP(a, b, vectorSize, curThreadsNum);
            double end = omp_get_wtime();
            time += end - start;
        }
        results.push_back(TestResult(curThreadsNum, (time / RETRIES) * 1000000));
        curThreadsNum += THREAD_STEP;
    }
    return results;
}

list<TestResult> test(int threadsNum) {
    int curVecSize = MIN_VECTOR_SIZE;
    int numOfIters = (MAX_VECTOR_SIZE - MIN_VECTOR_SIZE) / ITER_STEP;
    list<TestResult> results;       // результаты в виде <размер массива, время работы>
    for (int i = 0; i <= numOfIters; i++) {
        int* a = generateVector(curVecSize);
        int* b = generateVector(curVecSize);

        double start = omp_get_wtime();
        dotProductOMP(a, b, curVecSize, threadsNum);
        double end = omp_get_wtime();

        results.push_back(TestResult(curVecSize, (end - start)));
        curVecSize = curVecSize + ITER_STEP;
    }

    return results;
}


int main(int argc, char *argv[]) {
    // Thread tests
    cout << "Sequential algorithm time execution: " << testPerfDependsOnTheNumOfThreads(MAX_VECTOR_SIZE) << endl;
    list<TestResult> tres1 = testPerfDependsOnTheNumOfThreadsOMP(MAX_VECTOR_SIZE);
    saveTestResultsToFile(tres1, "thread_test.txt");
    // list<TestResult> res1 = test(MAX_THREADS);
    // saveTestResultsToFile(res1, "test_with_15_threads.txt");

    // list<TestResult> res2 = test(MIN_THREADS);
    // saveTestResultsToFile(res2, "test_with_5_threads.txt");
  
    // list<TestResult> res3 = test(CORES_NUM);
    // saveTestResultsToFile(res3, "test_with_10_threads.txt");
}

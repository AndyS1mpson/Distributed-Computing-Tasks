/*
Задача:
Вычислить определенный интеграл.
*/
#include <iostream>
#include <omp.h>
#include <math.h>
#include <list>
#include <fstream>
using namespace std;

#define MAX_THREADS 15        // больше числа ядер
#define MIN_THREADS 5         // меньше числа ядер
#define CORES_NUM 10          // равно числу ядер (Apple M1 Pro)


struct TestResult {

    TestResult(int threads, double execTime) {
        threadsNum = threads;
        executionTime = execTime;    
    };

    int threadsNum;
    double executionTime;
};

void saveTestResultsToFile(list<TestResult> testRestults, string filename) {
    ofstream file;
    file.open(filename);
    for (TestResult res : testRestults) {
      file << res.threadsNum << " " << res.executionTime<< endl;
    }
    file.close();
}

double f(double x) {
    return (x * x + 2 * x) * sin(2 * x);
}

double integrate(double(*f)(double), double a, double b, int n) {
    double dx = (b - a) / n;
    double res = 0.0;
    double x = a;
    for (int i = 0; i < n; i++) {
        res += f(x) * dx;
        x += dx;
    }
    return res;
}

double parallelizedInregrate(double(*f)(double), double a, double b, int n, int threadsNum) {
    double dx = (b - a) / n;
    double x = a;
    double sum = 0.0;
    #pragma omp parallel for shared(x,n,dx,f) reduction(+:sum) num_threads(threadsNum)
    for (int i = 0; i < n; i++) {
        sum += f(x + i * dx);
    }
    return sum * dx;
}

TestResult test(int threadsNum) {
    double a = 3;
    double b = 8;
    int n = 10;

    double start = omp_get_wtime();
    double res = parallelizedInregrate(f, a, b, n, threadsNum);
    double end = omp_get_wtime();

    return TestResult(threadsNum, end - start);
}

int main() {
    // double res = integrate(f, a, b, n);
    // double res = parallelizedInregrate(f, a, b, n, 10);

    list<TestResult> res;
    res.push_back(test(MIN_THREADS));
    res.push_back(test(CORES_NUM));
    res.push_back(test(MAX_THREADS));
    saveTestResultsToFile(res, "test_results.txt");
}
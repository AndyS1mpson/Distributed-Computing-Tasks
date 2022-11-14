/*
Задача:
Вычислить определенный интеграл.
*/
#include <iostream>
#include <omp.h>
#include <math.h>
using namespace std;

#define MAX_RAND_VAL 100000   // верхняя граница значений вектора
#define MAX_THREADS 15        // больше числа ядер
#define MIN_THREADS 5         // меньше числа ядер
#define CORES_NUM 10          // равно числу ядер (Apple M1 Pro)
#define MAX_ARRAY_SIZE 200000 // максимальный размер генерируемого вектора
#define MIN_ARRAY_SIZE 100000 // минимальный размер генерируемого вектора
#define ITER_STEP 20000       // шаг изменения размера вектора

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


int main() {
    double a = 3;
    double b = 8;
    int n = 10;
    // double res = integrate(f, a, b, n);
    double res = parallelizedInregrate(f, a, b, n, 10);
    cout << res << endl;
}
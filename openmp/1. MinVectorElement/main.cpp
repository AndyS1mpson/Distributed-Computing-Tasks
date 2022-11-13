#include <iostream>
#include <omp.h>
#include <vector>
#include <stdio.h>
using namespace std;

#define MAX_RAND_VAL 1000    // верхняя граница значений вектора
#define MAX_THREADS 32       // больше числа ядер
#define MIN_THREADS 5        // меньше числа ядер
#define CORES_NUM 8          // равно числу ядер
#define MAX_ARRAY_SIZE 50000 // максимальный размер генерируемого вектора
#define MIN_ARRAY_SIZE 5000  // минимальный размер генерируемого вектора
#define ITER_STEP 5000       // шаг изменения размера массива


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


pair<int, double>* test(int threads_num) {
    int vec_size = MIN_ARRAY_SIZE;

    int iter_count = (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE) / ITER_STEP;            // количество итераций/изменений размера массива
    pair<int, double>* results = new pair<int, double>[iter_count];            // результаты в виде <размер массива, время работы>
    for (int i = 0; i < iter_count; i++) {
        // генерируем массив
        int cur_vec_size = vec_size + i * ITER_STEP;
        int* vec = generateVector(cur_vec_size);

        double start = omp_get_wtime();
        findMin(vec, vec_size, threads_num);
        double end = omp_get_wtime();

        results[i].first = cur_vec_size;
        results[i].second = end - start;
    }
    return results;
}


int main(int argc, char *argv[]) {
    pair<int, double>* results = test(MAX_THREADS);
    for (int i = 0; i <= sizeof(results); i++) {
        cout << results[i].first << ", " << results[i].second << endl;
    }
}

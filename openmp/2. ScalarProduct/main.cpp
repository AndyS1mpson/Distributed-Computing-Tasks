/*
Задача:
Найти максимальный(минимальный) элемент массива.
*/
#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

#define MAX_RAND_VAL 100000   // верхняя граница значений вектора
#define MAX_THREADS 15       // больше числа ядер
#define MIN_THREADS 5        // меньше числа ядер
#define CORES_NUM 10         // равно числу ядер (Apple M1 Pro)
#define MAX_ARRAY_SIZE 200000 // максимальный размер генерируемого вектора
#define MIN_ARRAY_SIZE 100000  // минимальный размер генерируемого вектора
#define ITER_STEP 20000       // шаг изменения размера массива


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
    #pragma omp parallel for reduction(+:res)
    for (int i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    };
    return result;
}


int main(int argc, char *argv[]) {

}

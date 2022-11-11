#include <iostream>
#include <omp.h>
#include <vector>
#include <stdio.h>
using namespace std;

#define MAX_RAND_VAL 1000
#define MAX_THREADS 32 


int* generateVector(int size) {
    int* s = new int[size];
    srand(time(NULL));
    for (int i=0; i<size; i++) {
      s[i] = rand() % MAX_RAND_VAL + 1;
    };
    return s;
}

int findMin(int* vec, int n)
{
    int min = INT32_MAX;
    #pragma omp parallel for reduction(min:min)
    for (int i = 0; i < n; i++) {
        if (min >= vec[i])
            min = vec[i];
    }
    return min;
}

int main(int argc, char *argv[]) {
    // vector size
    int N;
    cout << "Please, enter n: " << endl;
    cin >> N;
    cout << endl;

    int* a = generateVector(N);

    for (int i=0; i<N; i++) {
      cout << a[i] << " ";
    }
    cout << endl;

    int min = findMin(a, N);
    
    cout << min << endl;

}

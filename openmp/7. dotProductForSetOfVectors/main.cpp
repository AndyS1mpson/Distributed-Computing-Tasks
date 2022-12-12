/*
Задача:
Разработать программу для вычисления скалярного произведения
для последовательного набора векторов (исходные данные брать из файла). 
*/
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define VECTOR_SIZE 1000
#define MAX_RAND_VAL 10000

const char* filename = "vectors";

int* generateVector(int size) {
    int* vec = new int[size];
    srand(time(NULL));
    for (int i=0; i<size; i++) {
      vec[i] = rand() % MAX_RAND_VAL + 1;
    };
    return vec;
}

void fillFileWithVectors(const char* filename, int numOfVec, int vecSize) {
    FILE* file;
    file = fopen(filename, "w");

    for(int i = 0; i < numOfVec; i++) {
        int* vec = generateVector(vecSize);
        for (int j = 0; j < vecSize; j++) {
			fprintf(file, "%d\t", vec[j]);
		}
		fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    fillFileWithVectors(filename, 3, 5);
}
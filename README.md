# Distributed-Computing-Tasks

## OpenMP

### Запуск OpenMP программ на MacOS:
1. Перейдите в директорию с конкретной задачей.
2. В консоли введите команды:
```
clang++ -Xpreprocessor -fopenmp main.cpp -o main -lomp 
./main
```

### Запуск OpenMP программы на Linux:
1. Перейдите в директорию с конкретной задачей.
2. В консоли введите следующие команды:
```
g++ -o main -fopenmp main.cpp
./main
```

## MPI
1. Перейдите в директорию с конкретной задачей.
2. В консоли введите команды:
```
mpic++ main.cpp -o main
mpirun -n *число потоков* main
```

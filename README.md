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
2. В консоли введите команду:
```
mpic++ main.cpp -o main
```
3. Создайте файл job.sh, содержащий следующие команды:
```
#!/bin/sh
#module add mpi/openmpi-local
module add openmpi
mpirun main
```
4. В консоли выполните следующую команду:
```
sbatch -n 4 job.sh
```
В результате в текущей директориибудет создан файл в котором будут логи выполнения программы

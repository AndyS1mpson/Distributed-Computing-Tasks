"""Plot OpenMP test results for different threads num."""
from typing import Tuple

import matplotlib.pyplot as plt


def read_coords_from_file(filepath: str) -> Tuple[int, float]:
    X, Y = [], []

    for line in open(filepath):
        values = line.split()
        X.append(int(values[0]))
        Y.append(float(values[1]))
    return X, Y


# def plot_test_result() -> None:
#     x_5, y_5 = read_coords_from_file('test_with_5_threads.txt')
#     x_10, y_10 = read_coords_from_file('test_with_10_threads.txt')
#     x_15, y_15 = read_coords_from_file('test_with_15_threads.txt')

#     plt.plot(x_5, y_5, label="5 threads")
#     plt.plot(x_10, y_10, label="10 threads")
#     plt.plot(x_15, y_15, label="15 theads")
#     plt.legend()
#     plt.savefig('plt.png')

def plot_thread_test_result() -> None:
    x, y = read_coords_from_file('thread_test.txt')

    plt.axhline(y=6060, color='orange',  linestyle = '-', label='Sequential')

    plt.plot(x, y, label = "OMP")
    plt.title("Время выполнения, микросекунды, N = 10^6")
    plt.scatter(x, y, color='blue', marker='o')
    plt.xlabel("Число потоков")
    plt.legend()
    plt.savefig('thread_test.png')

if __name__ == '__main__':
    # plot_test_result()
    plot_thread_test_result()

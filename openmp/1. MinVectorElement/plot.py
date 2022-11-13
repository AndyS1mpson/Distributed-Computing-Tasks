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


def plot_test_result() -> None:
    x_5, y_5 = read_coords_from_file('test_with_5_threads.txt')
    x_10, y_10 = read_coords_from_file('test_with_10_threads.txt')
    x_15, y_15 = read_coords_from_file('test_with_15_threads.txt')

    plt.plot(x_5, y_5, label="5 threads")
    plt.plot(x_10, y_10, label="10 threads")
    plt.plot(x_15, y_15, label="15 theads")
    plt.legend()
    plt.savefig('plt.png')


if __name__ == '__main__':
    plot_test_result()

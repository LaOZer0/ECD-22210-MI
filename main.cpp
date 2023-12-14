#include <iostream>
#define m 10

using namespace std;

void fillStraightList(int* array, int size) {
    for (int i = 0; i < size - 1; i++) {
        array[i] = i + 1;
    }
    array[size - 1] = 0;
}

void fillReversedList(int* array, int size) {
    for (int i = size - 1; i > 0; i--) {
        array[i] = i - 1;
    }
    array[0] = size - 1;
}

void fillRandomList(int* array, int size) {
    srand(time(nullptr));
    while (size > 1) {
        size -= 1;
        int j = rand() % size;
        swap(array[j], array[size]);
    }
}

void getAverageTime(const int* array, int size, const string& name) {
    volatile int k, i;
    for (k = 0, i = 0; i < size; i = i + 1) {
        k = array[k];
    }

    int min = 100000;

    for (int j = 0; j < m; j++) {
        unsigned long long start = __builtin_ia32_rdtsc();
        for (k = 0, i = 0; i < size * m; i = i + 1) {
            k = array[k];
        }
        unsigned long long end = __builtin_ia32_rdtsc();
        int time = (end - start) / (size * m);
        if (min > time) {
            min = time;
        }
    }
    cout << "Time taken:" << min << " " << name << ";  ";
}

int main() {
    int n_min = 256;
    int n_max = 32 * 1024 * 256;
    int k = 1;
    while (n_min <= n_max) {
        int* array = new int[n_min];
        cout << k << ") ";
        fillStraightList(array, n_min);
        getAverageTime(array, n_min, "Straight");


        fillReversedList(array, n_min);
        getAverageTime(array, n_min, "Reverse");


        fillRandomList(array, n_min);
        getAverageTime(array, n_min, "Random");


        cout << "\n";
        delete[]array;
        n_min *= 2;
        k++;
    }
    return 0;
}

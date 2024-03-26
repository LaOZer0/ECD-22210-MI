#pragma once

#include <iostream>

using namespace std;

class Matrix {
private:
    int N;
    double **matrix = nullptr;
public:
    explicit Matrix(int N);

    Matrix();

    ~Matrix();

    int getSize() const;

    double *operator[](size_t index);

    const double *operator[](size_t index) const;

    void show();
};
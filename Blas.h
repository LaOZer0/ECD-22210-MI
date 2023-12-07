//#include "cblas.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void mulMatrix(float* A, float* B, float* C, int N);

void sumMatrix(float* A, float* B, int N);

void subMatrix(float* A, float* B, float* C, int N);

void inverseMatrixWithBlas(float mas[], float BB[], int N, int M);

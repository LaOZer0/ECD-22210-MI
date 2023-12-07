#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <math.h>


float multiplicateMatrix(float* x, float* y, int N);

void multiplicateVectors(float A[], float B[], float C[], int N);

void addVectors(float A[], float B[], float C[], int N);

void subVectors(float A[], float B[], float C[], int N);

void inverseMatrixWithSSE(float mas[], float BB[], int N, int M);

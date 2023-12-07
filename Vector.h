#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <math.h>


float MulMatrix(float* x, float* y, int N);

void MulVectors(float A[], float B[], float C[], int N);

void AddVectors(float A[], float B[], float C[], int N);

void SubVectors(float A[], float B[], float C[], int N);

void InvertMatrixWithSSE(float mas[], float BB[], int N, int M);

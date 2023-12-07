#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include "Blas.h"
#include "Native.h"
#include "Vector.h"

int main() {
	int i, j, N = 2048, M = 10;
	float* mas = (float*)malloc(N * N * sizeof(float));
	float* res = (float*)malloc(N * N * sizeof(float));
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i == j)
			{
				mas[j * N + i] = 2.0;
			}
			else
			{
				mas[j * N + i] = 1.0;
			}
		}
	}

	inverseMatrixWithBlas(mas, res, N, M);
	inverseMatrixOfNative(mas, res, N, M);
	inverseMatrixWithSSE(mas, res, N, M);
}

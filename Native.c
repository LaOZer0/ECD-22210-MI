#include "Native.h"

void transposeMatrix(float matrix[], float transposed[], int N) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			transposed[j * N + i] = matrix[i * N + j];
		}
	}
}

void zeroMatrix(float matrix[], int N) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrix[j * N + i] = 0.0;
		}
	}
}

void mulmatrix(float A[], float B[], float C[], int N) {
	int i, j, k;
	zeroMatrix(C, N);
	float* transposedB = (float*)malloc(N * N * sizeof(float));
	transposeMatrix(B, transposedB, N);
	for (i = 0; i < N; i++) {
		for (k = 0; k < N; k++) {
			for (j = 0; j < N; j++)
			{
				C[i * N + j] += A[i * N + k] * transposedB[k * N + j];
			}
		}
	}
}

void summatrix(float A[], float B[], float C[], int N) {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			C[j * N + i] = A[j * N + i] + B[j * N + i];
		}
	}
}

void submatrix(float A[], float B[], float C[], int N) {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			C[j * N + i] = A[j * N + i] - B[j * N + i];
		}
	}
}

void InverseMatrixOfNative(float mas[], float BB[], int N, int M) {
	float* I, * B, * C, * R, * SIB, max, maxst, summa;
	int i, j, s;
	I = (float*)malloc(N * N * sizeof(float));
	B = (float*)malloc(N * N * sizeof(float));
	R = (float*)malloc(N * N * sizeof(float));
	C = (float*)malloc(N * N * sizeof(float));
	SIB = (float*)malloc(N * N * sizeof(float));
	max = 0;
	for (i = 0; i < N; i++) {
		summa = 0;
		for (j = 0; j < N; j++) {
			summa = summa + fabs(mas[i * N + j]);
		}
		if (summa > max) {
			max = summa;
		}
	}
	maxst = 0;
	for (j = 0; j < N; j++) {
		summa = 0;
		for (i = 0; i < N; i++) {
			summa = summa + fabs(mas[i * N + j]);
		}
		if (summa > maxst) {
			maxst = summa;
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			B[i * N + j] = mas[j * N + i] / (max * maxst);
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i == j)
			{
				I[i * N + j] = 1;
			}
			else
			{
				I[i * N + j] = 0;
			}
		}
	}

	mulmatrix(B, mas, C, N);

	submatrix(I, C, R, N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			BB[j * N + i] = I[j * N + i];
			SIB[j * N + i] = I[j * N + i];
		}
	}

	for (s = 1; s <= M; s++) {
		mulmatrix(SIB, R, C, N);

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				SIB[j * N + i] = C[j * N + i];
			}
		}
		summatrix(BB, SIB, BB, N);
	}
	mulmatrix(BB, B, C, N);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			BB[j * N + i] = C[j * N + i];
		}
	}
}

#include "Blas.h"

void mulMatrix(float* A, float* B, float* C, int N) {
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, A, N, B, N, 0.0, C, N);
}

void sumMatrix(float* A, float* B, int N) {
	for (int i = 0; i < N; i++) {
		cblas_saxpy(N, 1.0, &A[i * N], 1, &B[i * N], 1);
	}
}

void subMatrix(float* A, float* B, float* C, int N) {
	for (int i = 0; i < N; i++) {
		cblas_saxpy(N, -1.0, &A[i * N], 1, &B[i * N], 1);
		cblas_saxpy(N, -1.0, &B[i * N], 1, &C[i * N], 1);
	}
}

void inverseMatrixWithBlas(float mas[], float BB[], int N, int M) {
	float* I, * B, * C, * R, * sib;
	float max, maxst;
	int i, j, s;
	I = (float*)malloc(N * N * sizeof(float));
	B = (float*)malloc(N * N * sizeof(float));
	R = (float*)malloc(N * N * sizeof(float));
	C = (float*)malloc(N * N * sizeof(float));
	sib = (float*)malloc(N * N * sizeof(float));
	max = 0;
	for (int i = 0; i < N; ++i) {
		float sum = cblas_sasum(N, &mas[i * N], 1);
		if (sum > max) {
			max = sum;
		}
	}
	maxst = 0;
	for (int j = 0; j < N; ++j) {
		float sum = cblas_sasum(N, &mas[j], N);
		if (sum > maxst) {
			maxst = sum;
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			B[i * N + j] = mas[j * N + i] / (max * maxst);
			R[i * N + j] = 0;
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i == j)
			{
				I[j * N + i] = 1;
			}
			else
			{
				I[j * N + i] = 0;
			}
		}
	}

	mulMatrix(B, mas, C, N);

	subMatrix(I, C, R, N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			BB[j * N + i] = I[j * N + i];
			sib[j * N + i] = I[j * N + i];
		}
	}
	for (s = 1; s <= M; s++) {
		mulMatrix(sib, R, C, N);
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				sib[j * N + i] = C[j * N + i];
			}
		}

		sumMatrix(sib, BB, N);

	}
	mulMatrix(BB, B, C, N);
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			BB[j * N + i] = C[j * N + i];
		}
	}
}

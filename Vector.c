#include "Vector.h"

float MulMatrix(float* x, float* y, int N) {
	float summa;
	int i;
	__m128* xx, * yy;
	__m128 p = _mm_set_ps1(0), s = _mm_set_ps1(0);
	xx = (__m128*)x;
	yy = (__m128*)y;
	for (i = 0; i < N / 4; i++) {
		p = _mm_mul_ps(xx[i], yy[i]);
		s = _mm_add_ps(s, p);
	}
	p = _mm_movehl_ps(p, s);
	s = _mm_add_ps(s, p);
	p = _mm_shuffle_ps(s, s, 1);
	s = _mm_add_ss(s, p);
	_mm_store_ss(&summa, s);
	return summa;
}

void MulVectors(float A[], float B[], float C[], int N) {
	int i, j;
	float* mas = (float*)malloc(N * N * sizeof(float));
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			mas[j * N + i] = B[i * N + j];
		}
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			C[j * N + i] = MulMatrix(&A[i * N], &mas[j * N], N);
		}

	}
}

void AddVectors(float A[], float B[], float C[], int N) {
	int i, k;
	__m128 p;
	for (i = 0; i < N; i++) {
		__m128* xx, * yy;
		xx = (__m128*) & A[i * N];
		yy = (__m128*) & B[i * N];
		for (k = 0; k < N / 4; k++) {
			p = _mm_add_ps(xx[k], yy[k]);
			_mm_store_ps(&C[k * N + i * 4], p);
		}
	}
}

void SubVectors(float A[], float B[], float C[], int N) {
	int i, k;
	__m128 p;
	for (i = 0; i < N; i++) {
		for (k = 0; k < N / 4; k++) {
			__m128* xx, * yy;
			xx = (__m128*) & A[i * N];
			yy = (__m128*) & B[i * N];
			p = _mm_sub_ps(xx[k], yy[k]);
			_mm_store_ps(&C[k * N + i * 4], p);
		}
	}
}

void InvertMatrixWithSSE(float mas[], float BB[], int N, int M) {
	float* I, * B, * R, * C, * sib, max1, maxst;
	int summa, i, j;
	I = (float*)malloc(N * N * sizeof(float));
	B = (float*)malloc(N * N * sizeof(float));
	R = (float*)malloc(N * N * sizeof(float));
	C = (float*)malloc(N * N * sizeof(float));
	sib = (float*)malloc(N * N * sizeof(float));
	max1 = 0;

	for (i = 0; i < N; i++) {
		summa = 0;
		for (j = 0; j < N; j++) {
			summa = summa + fabs(mas[j * N + i]);
		}
		if (summa > max1) {
			max1 = summa;
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
			B[(j)*N + (i)] = mas[(i)*N + (j)] / (max1 * maxst);
		}
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i == j) {
				I[j * N + i] = 1;
			}
			else {
				I[j * N + i] = 0;
			}
		}
	}

	MulVectors(B, mas, C, N);

	SubVectors(I, C, R, N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			BB[j * N + i] = I[j * N + i];
			sib[j * N + i] = I[j * N + i];
		}
	}
	for (int s = 1; s <= M; s++) {
		MulVectors(sib, R, C, N);
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				sib[j * N + i] = C[j * N + i];
			}

		}
		AddVectors(BB, sib, BB, N);
	}
	MulVectors(BB, B, C, N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			BB[j * N + i] = C[j * N + i];
		}
	}
}

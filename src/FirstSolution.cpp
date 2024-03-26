#include "Solution.h"

void FirstSolution::changeTheVectorApproximation() {
    auto *new_x = new double[N];
    for (int i = blockBegin; i <= blockEnd; i++) {
        new_x[i] = vectorX[i] - tau * (result[i]);
    }
    MPI_Gather(&new_x[blockBegin],
               blockSize,
               MPI_DOUBLE,
               vectorX,
               blockSize,
               MPI_DOUBLE,
               0, MPI_COMM_WORLD);
    MPI_Bcast(vectorX, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    delete[] new_x;
}

bool FirstSolution::checkTheAccuracy(double epsilon) {
    for (int i = blockBegin, j = 0; j < blockSize; i++, j++) {
        blockResult[j] = multiplyVector(A[i], vectorX) - b[i];
    }
    MPI_Gather(blockResult,
               blockSize,
               MPI_DOUBLE,
               result,
               blockSize,
               MPI_DOUBLE,
               0,
               MPI_COMM_WORLD);
    MPI_Bcast(result, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double norm_numerator = findNorm(result, N);
    return norm_numerator / normDenominator < epsilon;
}

FirstSolution::FirstSolution(int N) : UsualSolution(N) {
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    blockSize = (N + size - 1) / size;

    blockBegin = rank * blockSize;
    blockEnd = size - 1 == rank ? N - 1 : blockSize * (rank + 1) - 1;
    blockSize = blockEnd - blockBegin + 1;
    blockResult = new double[blockSize];
    result = new double[N];
}

FirstSolution::~FirstSolution() {
    MPI_Barrier(MPI_COMM_WORLD);
}

void FirstSolution::printResult() {
    if (rank == 0) {
        for (int i = 0; i < 1; i++) {
            cout << vectorX[i] << ' ';
        }
        cout << endl;
    }
}

void FirstSolution::run(double epsilon) {
    int count = 0;
    while (!checkTheAccuracy(epsilon)) {
        changeTheVectorApproximation();
        count++;
    }
    if (rank == 0) cout << count << " cycles" << endl;
    delete[] result;
    delete[] blockResult;
    result = nullptr;
    blockResult = nullptr;
}
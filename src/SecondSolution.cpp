
#include "Solution.h"

#define FIRST_THREAD 0

SecondSolution::SecondSolution(int N) : UsualSolution(N) {
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        MPI_Finalize();
        throw runtime_error("Error: size is 1");
    }
    blockSize = (N + size - 1) / size;
    blockBegin = blockSize * rank;
    blockEnd = blockSize * (rank + 1) > N ? N : blockSize * (rank + 1);
    blockSize = blockEnd - blockBegin;
    destination = (rank + 1) % size;
    sender = (rank - 1 + size) % size;
    delete[] vectorX;
    delete[] b;
    vectorX = new double[blockSize];
    b = new double[blockSize];
    blockResult = new double[blockSize];
    tmp = new double[blockSize];
    result = new double[blockSize];
    for (int i = blockBegin, j = 0; i < blockEnd; i++, j++) {
        this->vectorX[j] = vectorX[i];
        this->b[j] = b[i];
    }
    normDenominator = findNormVectorB();
};

SecondSolution::~SecondSolution() {
    MPI_Barrier(MPI_COMM_WORLD);
}

double SecondSolution::findNormVectorB() {
    double sum_part = 0;
    double sum = 0;
    for (int i = 0; i < blockSize; i++) {
        sum_part += b[i] * b[i];
    }
    MPI_Allreduce(&sum_part, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return sum;
}

double SecondSolution::multiply_v(const double *a, const double *b, int offset, int count) const {
    double sum = 0.0;
    for (int i = offset, j = 0; i < offset + count; i++, j++) {
        sum += a[i] * b[j];
    }
    return sum;
}

void SecondSolution::run(double epsilon) {
    int count = 0;
    while (!checkTheAccuracy(epsilon)) {
        changeTheVectorApproximation();
        count++;
    }
    if (rank == 0) cout << count << " cycles" << endl;
    delete[] result;
    delete[] blockResult;
    delete[] tmp;
    result = nullptr;
    blockResult = nullptr;
    tmp = nullptr;
}

void SecondSolution::changeTheVectorApproximation() {
    for (int i = 0; i < blockSize; ++i) {
        blockResult[i] = 0;
    }
    for (int k = 0; k < size; k++) {
        int block = (rank + k) % size;
        for (int i = blockBegin, j = 0; i < blockEnd; i++, j++) {
            blockResult[j] += multiply_v(A[i], vectorX, block * blockSize, blockSize);
        }
        MPI_Sendrecv(&vectorX[0],
                     blockSize,
                     MPI_DOUBLE,
                     destination,
                     0,
                     &tmp[0],
                     blockSize,
                     MPI_DOUBLE,
                     sender,
                     MPI_ANY_TAG,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        for (int i = 0; i < blockSize; ++i) {
            vectorX[i] = tmp[i];
        }
    }
    for (int i = 0; i < blockSize; i++) {
        vectorX[i] = vectorX[i] - tau * (blockResult[i] - b[i]);
    }
}

bool SecondSolution::checkTheAccuracy(double epsilon) {
    for (int i = 0; i < blockSize; ++i) {
        result[i] = 0;
    }
    for (int k = 0; k < size; k++) {
        int block = (rank + k) % size;
        for (int i = blockBegin, j = 0; i < blockEnd; i++, j++) {
            result[j] += multiply_v(A[i], vectorX, block * blockSize, blockSize);
        }
        MPI_Sendrecv(vectorX,
                     blockSize,
                     MPI_DOUBLE,
                     destination,
                     0,
                     tmp,
                     blockSize,
                     MPI_DOUBLE,
                     sender,
                     MPI_ANY_TAG,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        for (int i = 0; i < blockSize; ++i) {
            vectorX[i] = tmp[i];
        }
    }
    double partNorm = 0;
    for (int i = 0; i < blockSize; i++) {
        partNorm += (result[i] - b[i]) * (result[i] - b[i]);
    }
    double normNumerator;
    MPI_Allreduce(&partNorm,
                  &normNumerator,
                  1,
                  MPI_DOUBLE,
                  MPI_SUM,
                  MPI_COMM_WORLD);
    return normNumerator / normDenominator < epsilon * epsilon;
}

void SecondSolution::printResult() {
    int global_x_size = 0;
    int part_x_size = blockSize;
    MPI_Allreduce(&part_x_size,
                  &global_x_size,
                  1,
                  MPI_INT,
                  MPI_SUM,
                  MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Count of elements X = " << global_x_size << endl;
    }
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            for (int i = 0; i < blockSize; i++) {
                cout << vectorX[i] << ' ';
            }
        }
    }
    if (rank == 0) {
        cout << endl;
    }
}
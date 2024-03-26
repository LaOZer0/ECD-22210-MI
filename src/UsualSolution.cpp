#include "Solution.h"

double UsualSolution::tau = 0.00001;

static int n = 0;

void UsualSolution::printVector(double *v) const {
    for (int i = 0; i < N; ++i) {
        cout << v[i] << ' ';
    }
    cout << endl;
}

double UsualSolution::multiplyVector(const double *a, const double *b) const {
    double result = 0.0;
    for (size_t i = 0; i < N; ++i) {
        result += a[i] * b[i];
    }
    return result;
}

UsualSolution::UsualSolution(int N) : A(N) {
    this->N = N;
    b = new double[N];
    vectorX = new double[N];
    for (size_t i = 0; i < N; ++i) {
        b[i] = N + 1;
    }
    for (size_t i = 0; i < N; ++i) {
        vectorX[i] = 0;
    }
    normDenominator = findNorm(b, N);
}

double UsualSolution::findNorm(const double *v, int size) const {
    double result = 0.0;
    for (int i = 0; i < size; i++) {
        result += v[i] * v[i];
    }
    result = pow(result, 0.5);
    return result;
}

void UsualSolution::run(double epsilon) {
    while (!checkTheAccuracy(epsilon)) {
        changeTheVectorApproximation();
    }
}

void UsualSolution::changeTheVectorApproximation() {
    double *new_x = new double[N];
    for (int i = 0; i < N; i++) {
        new_x[i] = multiplyVector(A[i], vectorX);
    }

    for (int i = 0; i < N; i++) {
        vectorX[i] = vectorX[i] - tau * (new_x[i] - b[i]);
    }
    delete[] new_x;
}

bool UsualSolution::checkTheAccuracy(double epsilon) {
    double *result = new double[N];
    for (int i = 0; i < N; i++) {
        result[i] = multiplyVector(A[i], vectorX);
    }

    for (int i = 0; i < N; i++) {
        result[i] = (result[i] - b[i]);
    }
    double normNumerator = findNorm(result, N);
    delete[] result;
    return normNumerator / normDenominator < epsilon;
}

void UsualSolution::printResult() {
    for (int i = 0; i < N; i++) {
        cout << vectorX[i] << ' ';
    }
    cout << endl;
}

UsualSolution::UsualSolution() {

}

UsualSolution::~UsualSolution() {
    delete[] vectorX;
    delete[] b;
}
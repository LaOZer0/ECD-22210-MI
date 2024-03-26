#pragma once

#include "Matrix.h"
#include <iostream>
#include <cmath>
#include <mpi.h>

#define MAIN_THREAD 0

class UsualSolution {
private:
    UsualSolution();

protected:
    static double tau;
    int N;
    double *b;
    double *vectorX;
    Matrix A;
    double normDenominator = 0;

    virtual bool checkTheAccuracy(double epsilon);

    virtual double multiplyVector(const double *a, const double *b) const;

    double findNorm(const double *v, int size) const;

    virtual void changeTheVectorApproximation();


public:

    void printVector(double *v) const;

    virtual ~UsualSolution();

    explicit UsualSolution(int N);

    virtual void run(double epsilon);

    virtual void printResult();
};

class FirstSolution : public UsualSolution {
private:
    int size, rank, blockBegin, blockEnd, blockSize;

    void changeTheVectorApproximation() override;

    bool checkTheAccuracy(double epsilon) override;

    double *blockResult = nullptr;
    double *result = nullptr;
public:
    explicit FirstSolution(int N);

    ~FirstSolution();

    void printResult() override;

    void run(double epsilon) override;
};

class SecondSolution : public UsualSolution {
private:
    int size, rank, blockBegin, blockEnd, blockSize, destination, sender;

    void changeTheVectorApproximation() override;

    bool checkTheAccuracy(double epsilon) override;

    double multiplyVector(const double *a, const double *b, int offset, int count) const;

    double findNormVectorB();

    double *blockResult = nullptr;
    double *result = nullptr;
    double *tmp = nullptr;
public:
    explicit SecondSolution(int N);

    ~SecondSolution();

    void printResult() override;

    void run(double epsilon) override;
};
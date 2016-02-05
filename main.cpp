#include "lib/matrixman.h"
#include <iostream>
using namespace MatrixMan;

int main()
{
    Matrix<double> A(10,10,2);
    Matrix<double> X = eye(10);

    X.slice(1,3,1,3) += 5;
    A[5][3] = 7;
    A[0][0] = -1;
    A[5][1] = -4;
    X[3][8] = 11;
    X[2][4] = -2;
    X[9][9] = 3;

    X.display();

    X[2].display();
    (X||2). display();

    Matrix<double> Z = A.slice(3,7,3,7);
    return 0;
}

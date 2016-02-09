#include "lib/matrixman.h"
#include <iostream>
#include <cmath>
#define EPS 0.0000000001
using namespace MatrixMan;

int main()
{
    Matrix<double> A(10,10,2);
	Matrix<double> X = eye(10);
	Matrix<double> D(8, 4, 1);


    X.sub(1,3,1,3) += 5;
    A[5][3] = 7;
    A[0][0] = -1;
    A[5][1] = -4;

	D[0][1] = -0.000000001;
	D[0][2] = 0.0000000005;
	D[1][3] = -6;
	D[2][0] = 2;

    X[3][8] = 11;
    X[2][4] = -2;
    X[9][9] = 3;

	D.transpose().display();
	D.zeroIfLowerThan(0.00001).display();
	Matrix<double> C = D.transpose();

    X.display();

    X[2].display();
    (X||2).display();
    (ones(5,5)*4).tril().display();
    (ones(5,5)*3).triu().display();

    Matrix<double> Z = A.sub(3,7,3,7);

	RowSlice<double> RS(&D.transpose(), 0, 0, 5);
	ColumnSlice<double> CS(&D, 0, 0, 5);

	std::cout << RS*CS << "\n";
	(CS*RS).display();

    return 0;
}

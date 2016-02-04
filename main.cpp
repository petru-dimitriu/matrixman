#include "matrixman.h"
#include "row.h"
#include "slice.h"
#include <iostream>
using namespace MatrixMan;

int main()
{
    Matrix<double> A(10,10,2);
    /*Matrix<double> B(3,3,5);
    B[2][1] += 1;
    A[1].fill(2);
    A.slice(1,3,1,3) = B;
    A.slice(5,-1,4,9).fill(3).slice(1,2,1,2).fill(7);

    A.display();
    std::cout << "--\n";
    Matrix<double> C = A*20;
    C = C/5.2;
    C.display();
    A||4 = 5;
    A||8 = 8;
    (A||8).display();
    */
    Matrix<double> X = eye(5);
    X||2 = X || 4;
    X||1 = 7;
    X[2] = 5;
    X.display();

    return 0;
}

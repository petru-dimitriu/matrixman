#pragma once
#include "mmerror.h"
#include "slice.h"
#include "row.h"
#include "column.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

namespace MatrixMan
{
    template <class T>
    class RowSlice;

    template <class T>
    class Slice;

    template <class T>
    class Matrix
    {
        friend class RowSlice<T>;
        friend class ColumnSlice<T>;
        friend class Slice<T>;

        private:
            T** matrix;
            int rows, cols;
            static std::vector<Matrix<T>*> dyn;

            void allocate(int rows, int cols)
            {
                matrix = new T*[rows];
                for (int i=0;i<rows;i++)
                    matrix[i] = new T[cols];
            }

            void deallocate()
            {
                for (int i=0;i<cols;i++)
                    delete matrix[i];
                delete[] matrix;
            }

        public:

            static void collectGarbage()
            {
                while (dyn.size() != 0)
                {
                    delete dyn.at(0);
                    dyn.erase(dyn.begin());
                }
            }

            Matrix()
            {
                matrix = NULL;
                this->rows = 0;
                this->cols = 0;
            }

            Matrix(int rows, int cols, T initialValue, bool dynamic = 0)
            {
                allocate(rows,cols);
                this->rows = rows;
                this->cols = cols;
                fill (initialValue);

                if (dynamic)
                    dyn.push_back(this);
            }

            Matrix(RowSlice<T>& X, bool dynamic = 0)
            {
                this->rows = 1;
                this->cols = X.from_col-X.to_col+1;
                allocate(rows,cols);
                for (int i=0;i<cols;i++)
                    matrix[0][i] = X.get(i);
                if (dynamic)
                    dyn.push_back(this);
            }

            Matrix(ColumnSlice<T>& X, bool dynamic = 0)
            {
                this->rows = X.to_row-X.from_row+1;
                this->cols = 1;
                this->dynamic = dynamic;
                allocate(rows,cols);
                for (int i=0;i<rows;i++)
                    matrix[i][0] = X.get(i);
                if (dynamic)
                    dyn.push_back(this);
            }

            Matrix(Slice<T>& X, bool dynamic = 0)
            {
                rows = X.rows;
                cols = X.cols;
                this->dynamic = dynamic;
                allocate(rows,cols);
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] = X.get(i,j);
                if (dynamic)
                    dyn.push_back(this);
            }

            ~Matrix()
            {
                //std::cout << "Dealocate!!\n";
                deallocate();
            }

            Matrix<T>& display()
            {
                for (int i=0;i<rows;i++)
                {
                    for (int j=0;j<cols;j++)
                        std::cout << matrix[i][j] << " ";
                    std::cout << "\n";
                }
                return *this;
            }

            Matrix<T>& fill(T what)
            {
                int i,j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] = what;
                return *this;
            }


			Matrix<T>& operator~()
			{
				return transpose();
			}

            RowSlice<T>& operator[] (int x)
            {
                return *(new RowSlice<T>(this,x,-1,-1,1));
            }

            ColumnSlice<T>& operator|| (int x)
            {
                return *(new ColumnSlice<T>(this,x,-1,-1,1));
            }

            Slice<T>& sub(int from_row, int to_row, int from_col, int to_col)
            {
                return *(new Slice<T>(this,from_row,to_row,from_col,to_col));
            }

            Matrix (const Matrix<T>& m, bool dynamic = 0)
            {
                rows = m.rows;
                cols = m.cols;
                allocate(rows,cols);
                int i, j;
                for (i=0;i<m.rows;i++)
                    for (j=0;j<m.cols;j++)
                        matrix[i][j] = m.matrix[i][j];
                if (dynamic)
                    dyn.push_back(this);
            }

            Matrix (const ColumnSlice<T>& m)
            {
                rows = m.to_row - m.from_row + 1;
                allocate(rows,1);
                for (int i=0;i<rows;i++)
                    matrix[i][0] = m.matrix[i][m.mat_col];
            }

            /*** OPERATIONS WITH SCALARS ***/

            Matrix<T>& operator= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] = t;
                collectGarbage();
            }


            Matrix<T>& operator+ (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this,1);
                (*A) += t;
                return *A;
            }

            Matrix<T>& operator- (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this,1);
                (*A) -= t;
                return *A;
            }

            Matrix<T>& operator* (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this,1);
                (*A) *= t;
                return *A;
            }

            Matrix<T>& operator/ (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this,1);
                (*A) /= t;
                return *A;
            }

            Matrix<T>& operator += (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] += t;
                return *this;
            }

            Matrix<T>& operator -= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] -= t;
                return *this;
            }

            Matrix<T>& operator *= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] *= t;
                return *this;
            }

            Matrix<T>& operator /= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] /= t;
                return *this;
            }

            /*** OPERATIONS WITH OTHER MATRICES ***/

            Matrix<T>& operator += (const Matrix<T>& X)
            {
                if (X.rows != rows || X.cols != cols)
                {
                    throw MMError("Matrix dimensions do not match.");
                    return *this;
                }
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] += X.matrix[i][j];
                return *this;
            }

            Matrix<T>& operator -= (const Matrix<T>& X)
            {
                if (X.rows != rows || X.cols != cols)
                {
                    throw MMError("Matrix dimensions do not match.");
                    return *this;
                }
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] -= X.matrix[i][j];
                return *this;
            }


            Matrix<T>& operator = (const Matrix<T>& X)
            {
                deallocate();
                allocate(X.rows,X.cols);
                rows = X.rows;
                cols = X.cols;
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] = X.matrix[i][j];
                collectGarbage();
                return *this;
            }

            Matrix<T>& operator+ (const Matrix<T>& X)
            {
                Matrix<T>* A = new Matrix<T>(*this,1);
                (*A) += X;
                return *A;
            }

            Matrix<T>& operator- (const Matrix<T>& X)
            {
                Matrix<T>* A = new Matrix<T>(*this,1);
                (*A) -= X;
                return *A;
            }

            Matrix<T>& operator* (const Matrix<T>& X)
            {
                Matrix<T>* A = new Matrix<T>(*this,1);
                (*A) *= X;
                return *A;
            }

            Matrix<T>& operator*= (const Matrix<T>& X)
            {
                if (cols != X.rows)
                    throw new MMError ("Matrix dimensions do not match for multiplication.");
                Matrix<T>* A = new Matrix<T>(rows,X.cols,0);
                int i,j,k;
                for (i=0;i<rows;i++)
                    for (j=0;j<X.cols;j++)
                    {
                        for (k=0;k<cols;k++)
                        {
                            A->matrix[i][j] += matrix[i][k] * X.matrix[k][j];
                        }
                    }
                *this = *A;
                return *this;
            }

            Matrix<T>& strassenMultiply(Matrix<T>& X)
            {
                if (cols != X.cols || cols != rows || rows != X.rows)
                    throw new MMError ("Matrix dimensions do not match for multiplication.");
                if (cols == 2)
                {
                    Matrix<T>* newMatrix = new Matrix<T>(2,2,0,1);
                    T m[7];
                    m[0] = (matrix[0][1] - matrix[1][1])*(X.matrix[1][0]+X.matrix[1][1]);
                    m[1] = (matrix[0][0] + matrix[1][1])*(X.matrix[0][0]+X.matrix[1][1]);
                    m[2] = (matrix[0][0] - matrix[1][0])*(X.matrix[0][0]+X.matrix[0][1]);
                    m[3] = (matrix[0][0] + matrix[0][1])*X.matrix[1][1];
                    m[4] = matrix[0][0] * (X.matrix[0][1] - X.matrix[1][1]);
                    m[5] = matrix[1][1] * (X.matrix[1][0] - X.matrix[0][0]);
                    m[6] = (matrix[1][0]+matrix[1][1])*X.matrix[0][0];
                    newMatrix->get(0,0) = m[0] + m[1] - m[3] + m[5];
                    newMatrix->get(0,1) = m[3] + m[4];
                    newMatrix->get(1,0) = m[5] + m[6];
                    newMatrix->get(1,1) = m[1] - m[2] + m[4] - m[6];
                    return *newMatrix;
                }
                else
                {
                    Matrix<T>* C = new Matrix<T>(rows,cols,0);
                    Matrix<T> A11 = sub(0,rows/2-1,0,cols/2-1);
                    Matrix<T> A12 = sub(0,rows/2-1,cols/2,cols-1);
                    Matrix<T> A21 = sub(rows/2,rows-1,0,cols/2-1);
                    Matrix<T> A22 = sub(rows/2,rows-1,cols/2,cols-1);

                    Matrix<T> B11 = X.sub(0,rows/2-1,0,cols/2-1);
                    Matrix<T> B12 = X.sub(0,rows/2-1,cols/2,cols-1);
                    Matrix<T> B21 = X.sub(rows/2,rows-1,0,cols/2-1);
                    Matrix<T> B22 = X.sub(rows/2,rows-1,cols/2,cols-1);

                    Matrix<T> M1, M2, M3, M4, M5, M6, M7;
                    M1 = (A12 - A22).strassenMultiply(B21 + B22);
                    M2 = (A11 + A22).strassenMultiply(B11 + B22);
                    M3 = (A11 - A21).strassenMultiply(B11 + B12);
                    M4 = (A11 + A12).strassenMultiply(B22);
                    M5 = A11.strassenMultiply(B12 - B22);
                    M6 = A22.strassenMultiply(B21 - B11);
                    M7 = (A21 + A22).strassenMultiply(B11);

                    C->sub(0,rows/2-1,0,cols/2-1) = M1 + M2 - M4 + M6;
                    C->sub(0,rows/2-1,cols/2,cols-1) = M4 + M5;
                    C->sub(rows/2,rows-1,0,cols/2-1) = M6 + M7;
                    C->sub(rows/2,rows-1,cols/2,cols-1) = M2 - M3 + M5 - M7;

                    return *C;
                }
            }

            Matrix<T>& operator*= (const Slice<T>& X)
            {
                if (cols != X.rows)
                    throw new MMError ("Matrix dimensions do not match for multiplication.");
                Matrix<T>* A = new Matrix<T>(rows,X.cols,0,1);
                int i,j,k;
                for (i=0;i<rows;i++)
                    for (j=0;j<X.cols;j++)
                    {
                        for (k=0;k<cols;k++)
                        {
                            A->matrix[i][j] += matrix[i][k] * X.get(k,j);
                        }
                    }
                *this = *A;
                return *this;
            }

            Matrix<T>& diag()
            {
                Matrix<T>* newMatrix = new Matrix<T>(rows,cols,0,1);
                int x = std::min(rows, cols), i;
                for (i=0;i<x;i++)
                    newMatrix->matrix[i][i] = matrix[i][i];
                return *newMatrix;
            }

            Matrix<T>& diagCol()
            {
                int x = std::min(rows, cols), i;
                Matrix<T>* newMatrix = new Matrix<T>(x,1,0,1);
                for (i=0;i<x;i++)
                    newMatrix->matrix[i][0] = matrix[i][i];
                return *newMatrix;
            }

            bool isDiag()
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        if (i!=j && matrix[i][j] != 0)
                            return false;
                return true;
            }

            Matrix<T>& tril()
            {
                Matrix<T>* newMatrix = new Matrix<T>(rows,cols,0,1);
                int x = std::min(rows, cols), i, j;
                for (i=0;i<x;i++)
                    for (j=0;j<=i;j++)
                        newMatrix->matrix[i][j] = get(i,j);
                return *newMatrix;
            }

            bool isTril()
            {
                int i, j;
                int x = std::min(rows, cols);
                for (i=0;i<x;i++)
                    for (j=i+1;j<cols;j++)
                        if (i!=j && matrix[i][j] != 0)
                            return false;
                return true;
            }

            Matrix<T>& triu()
            {
                Matrix<T>* newMatrix = new Matrix<T>(rows,cols,0,1);
                int x = std::min(rows, cols), i, j;
                for (i=0;i<x;i++)
                    for (j=i;j<cols;j++)
                        newMatrix->matrix[i][j] = get(i,j);
                return *newMatrix;
            }

            bool isTriu()
            {
                int i, j;
                int x = std::min(rows, cols);
                for (i=0;i<x;i++)
                    for (j=0;j<i;j++)
                        if (i!=j && matrix[i][j] != 0)
                            return false;
                return true;
            }

            inline T& get(int x, int y) const
            {
                return matrix[x][y];
            }

			// ---------------------------------------------------------------------------------------

			Matrix<T>& transpose()
			{
				Matrix<T>* newMatrix = new Matrix<T>(cols, rows, 0,1);
				for (int i = 0; i < rows; ++i)
					for (int j = 0; j < cols; ++j)
						newMatrix->matrix[j][i] = get(i,j);
				return *newMatrix;
			}

			Matrix<T>& zeroIfLowerThan(long double eps = 0.0000000001)
			{
				eps = std::fabs(eps);

				Matrix<T>* newMatrix = new Matrix<T>(rows, cols, 0,1);
				for (int i = 0; i < rows; ++i)
					for (int j = 0; j < cols; ++j)
						newMatrix->matrix[i][j] = (std::fabs(get(i, j)) > eps) ? get(i, j) : 0;
				return *newMatrix;
			}
    };


    Matrix<double>& ones(int x, int y)
    {
        return *(new Matrix<double>(x,y,1,1));
    }

    Matrix<double>& eye(int x)
    {
        Matrix<double>* R = new Matrix<double> (x,x,0,1);
        do
            R->get(x-1,x-1) = 1;
        while (--x);
        return *R;
    }
};

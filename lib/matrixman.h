#pragma once
#include "mmerror.h"
#include "slice.h"
#include "row.h"
#include "column.h"
#include <iostream>
#include <algorithm>

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
            Matrix()
            {
                matrix = NULL;
                this->rows = 0;
                this->cols = 0;
            }

            Matrix(int rows, int cols, T initialValue)
            {
                allocate(rows,cols);
                this->rows = rows;
                this->cols = cols;
                fill (initialValue);
            }

            Matrix(RowSlice<T>& X)
            {
                this->rows = 1;
                this->cols = X.from_col-X.to_col+1;
                allocate(rows,cols);
                for (int i=0;i<cols;i++)
                    matrix[0][i] = X.get(i);
            }

            Matrix(ColumnSlice<T>& X)
            {
                this->rows = X.to_row-X.from_row+1;
                this->cols = 1;
                allocate(rows,cols);
                for (int i=0;i<rows;i++)
                    matrix[i][0] = X.get(i);
            }

            Matrix(Slice<T>& X)
            {
                rows = X.rows;
                cols = X.cols;
                allocate(rows,cols);
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] = X.get(i,j);
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


            RowSlice<T>& operator[] (int x)
            {
                return *(new RowSlice<T>(this,x,-1,-1));
            }

            ColumnSlice<T>& operator|| (int x)
            {
                return *(new ColumnSlice<T>(this,x,-1,-1));
            }

            Slice<T>& sub(int from_row, int to_row, int from_col, int to_col)// -------------------------------------------------------------- FUNCTIE SLICE
            {
                return *(new Slice<T>(this,from_row,to_row,from_col,to_col));
            }

            Matrix (const Matrix<T>& m)
            {
                rows = m.rows;
                cols = m.cols;
                allocate(rows,cols);
                int i, j;
                for (i=0;i<m.rows;i++)
                    for (j=0;j<m.cols;j++)
                        matrix[i][j] = m.matrix[i][j];
            }

            /*** OPERATIONS WITH SCALARS ***/

            Matrix<T>& operator= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[i][j] = t;
            }


            Matrix<T>& operator+ (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this);
                (*A) += t;
                return *A;
            }

            Matrix<T>& operator- (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this);
                (*A) -= t;
                return *A;
            }

            Matrix<T>& operator* (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this);
                (*A) *= t;
                return *A;
            }

            Matrix<T>& operator/ (const T& t)
            {
                Matrix<T>* A = new Matrix<T>(*this);
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
                return *this;
            }

            Matrix<T>& operator+ (const Matrix<T>& X)
            {
                Matrix<T>* A = new Matrix<T>(*this);
                (*A) += X;
                return *A;
            }

            Matrix<T>& operator- (const Matrix<T>& X)
            {
                Matrix<T>* A = new Matrix<T>(*this);
                (*A) -= X;
                return *A;
            }

            Matrix<T>& operator* (const Matrix<T>& X)
            {
                Matrix<T>* A = new Matrix<T>(*this);
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

            Matrix<T>& operator*= (const Slice<T>& X)
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
                            A->matrix[i][j] += matrix[i][k] * X.get(k,j);
                        }
                    }
                *this = *A;
                return *this;
            }

            Matrix<T>& diag()
            {
                Matrix<T>* newMatrix = new Matrix<T>(rows,cols,0);
                int x = std::min(rows, cols), i;
                for (i=0;i<x;i++)
                    newMatrix->matrix[i][i] = matrix[i][i];
                return *newMatrix;
            }

            Matrix<T>& diagCol()
            {
                int x = std::min(rows, cols), i;
                Matrix<T>* newMatrix = new Matrix<T>(x,1,0);
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
                Matrix<T>* newMatrix = new Matrix<T>(rows,cols,0);
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
                Matrix<T>* newMatrix = new Matrix<T>(rows,cols,0);
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
    };

    Matrix<double>& ones(int x, int y)
    {
        return *(new Matrix<double>(x,y,1));
    }

    Matrix<double>& eye(int x)
    {
        Matrix<double>* R = new Matrix<double> (x,x,0);
        do
            R->get(x-1,x-1) = 1;
        while (--x);
        return *R;
    }
};

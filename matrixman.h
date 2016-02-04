#pragma once
#include "mmerror.h"
#include "slice.h"
#include "row.h"
#include <iostream>

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
            }

            Matrix(int rows, int cols, T initialValue)
            {
                allocate(rows,cols);
                this->rows = rows;
                this->cols = cols;
                fill (initialValue);
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

            Slice<T>& slice(int from_row, int to_row, int from_col, int to_col)
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

            T& get(int x, int y)
            {
                return matrix[x][y];
            }
    };
};

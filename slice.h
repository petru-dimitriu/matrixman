#pragma once
#include <iostream>
#include "mmerror.h"
#include "slice.h"

namespace MatrixMan
{
    template <class T>
    class RowSlice;

    template <class T>
    class Matrix;

    template <class T>
    class Slice
    {
        friend class Matrix<T>;

        private:
            T** matrix;
            Matrix<T>* m;
            int rows, cols;
            int from_row, from_col, to_row, to_col;

        public:
            Slice();

            Slice(Matrix<T>* m, int from_row, int to_row, int from_col, int to_col)
            {

                if (from_row < -1 || from_col < -1 ||
                    from_row > m->rows-1 || from_col > m->cols-1 ||
                    to_row < -1 || to_col < -1 ||
                    to_row > m->rows-1 || to_col > m->cols-1)
                    {
                        throw MMError("Index out of bounds");
                    }


                this->from_row = from_row == -1 ? 0 : from_row;
                this->to_row = to_row == -1 ? m->rows-1 : to_row;
                this->from_col = from_col == -1 ? 0 : from_col;
                this->to_col = to_col == -1 ? m->cols-1 : to_col;
                rows = to_row - from_row + 1;
                cols = to_col - from_col + 1;
                matrix = m->matrix;
                this->m = m;
            }

            Slice(const Slice<T>& s)
            {
                matrix = s.matrix;
                m = s.m;
                rows = s.rows;
                cols = s.cols;
                from_row = s.from_row;
                from_col = s.from_col;
                to_row = s.to_row;
                to_col = s.to_col;
            }

            Slice<T>& display()
            {
                for (int i=from_row;i<=to_row;i++)
                {
                    for (int j=from_col;j<=to_col;j++)
                        std::cout << matrix[i][j] << " ";
                    std::cout << "\n";
                }
                return *this;
            }

            Slice<T>& fill(T what)
            {
                int i,j;
                for (i=from_row;i<=to_row;i++)
                    for (j=from_col;j<=to_col;j++)
                        matrix[i][j] = what;
                return *this;
            }

            Slice<T>& operator= (const T& what)
            {
                return fill(what);
            }

            RowSlice<T>& operator[] (int x)
            {
                get(x);
            }

            /*** OPERATIONS WITH SCALARS ***/

            Slice<T>& operator+ (const T& t)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) += t;
                return *A;
            }

            Slice<T>& operator- (const T& t)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) -= t;
                return *A;
            }

            Slice<T>& operator* (const T& t)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) *= t;
                return *A;
            }

            Slice<T>& operator/ (const T& t)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) /= t;
                return *A;
            }

            Slice<T>& operator += (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[from_row+i][from_col+j] += t;
                return *this;
            }

            Slice<T>& operator -= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[from_row+i][from_col+j] -= t;
                return *this;
            }

            Slice<T>& operator *= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[from_row+i][from_col+j] *= t;
                return *this;
            }

            Slice<T>& operator /= (const T& t)
            {
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[from_row+i][from_col+j] /= t;
                return *this;
            }

            /*** OPERATIONS WITH OTHER MATRICES ***/

            Slice<T>& operator += (const Matrix<T>& X)
            {
                if (X.rows != rows || X.cols != cols)
                {
                    throw MMError("Matrix dimensions do not match.");
                    return *this;
                }
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[from_row+i][from_col+j] += X.matrix[i][j];
                return *this;
            }

            Slice<T>& operator+= (Matrix<T> &X)
            {
                if (X.rows != (to_row - from_row + 1) ||
                    X.cols != (to_col - from_col + 1))
                    {
                        std::cout << from_row << " " << X.cols << "\n";
                        throw MMError("Matrix dimensions do not match.");
                        return *this;
                    }

                T* buffer = new T[(X.rows)*(X.cols)];
                int i, j;
                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        buffer[(i*X.cols)+j] = matrix[from_row+i][from_col+j] + X[i][j];

                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        (matrix[from_row+i][from_col+j] = buffer[(i*X.cols)+j]);

                delete[] buffer;
            }

            Slice<T>& operator-= (Matrix<T> &X)
            {
                if (X.rows != (to_row - from_row + 1) ||
                    X.cols != (to_col - from_col + 1))
                    {
                        std::cout << from_row << " " << X.cols << "\n";
                        throw MMError("Matrix dimensions do not match.");
                        return *this;
                    }

                T* buffer = new T[(X.rows)*(X.cols)];
                int i, j;
                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        buffer[(i*X.cols)+j] = matrix[from_row+i][from_col+j] - X[i][j];

                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        (matrix[from_row+i][from_col+j] = buffer[(i*X.cols)+j]);

                delete[] buffer;
            }

            Slice<T>& operator+ (const Matrix<T>& X)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) += X;
                return *A;
            }

            Slice<T>& operator- (const Matrix<T>& X)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) -= X;
                return *A;
            }

            Slice<T>& operator* (const Matrix<T>& X)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) *= X;
                return *A;
            }

            Slice<T>& operator= (Matrix<T> &X)
            {
                if (X.rows != (to_row - from_row + 1) ||
                    X.cols != (to_col - from_col + 1))
                    {
                        std::cout << from_row << " " << X.cols << "\n";
                        throw MMError("Matrix dimensions do not match.");
                        return *this;
                    }

                T* buffer = new T[(X.rows)*(X.cols)];
                int i, j;
                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        buffer[(i*X.cols)+j] = X[i][j];

                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        (matrix[from_row+i][from_col+j] = buffer[(i*X.cols)+j]);

                delete[] buffer;
            }

            /*** OPERATIONS WITH OTHER SLICES ***/
            Slice<T>& operator += (const Slice<T> & X)
            {
                if (X.rows != rows || X.cols != cols)
                {
                    throw MMError("Matrix dimensions do not match.");
                    return *this;
                }
                int i, j;
                for (i=0;i<rows;i++)
                    for (j=0;j<cols;j++)
                        matrix[from_row+i][from_col+j] += X.get(i,j);
                return *this;
            }

            Slice<T>& operator+= (Slice<T>  &X)
            {
                if (X.rows != (to_row - from_row + 1) ||
                    X.cols != (to_col - from_col + 1))
                    {
                        std::cout << from_row << " " << X.cols << "\n";
                        throw MMError("Matrix dimensions do not match.");
                        return *this;
                    }

                T* buffer = new T[(X.rows)*(X.cols)];
                int i, j;
                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        buffer[(i*X.cols)+j] = matrix[from_row+i][from_col+j] + X.get(i,j);

                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        (matrix[from_row+i][from_col+j] = buffer[(i*X.cols)+j]);

                delete[] buffer;
            }

            Slice<T>& operator-= (Slice<T> &X)
            {
                if (X.rows != (to_row - from_row + 1) ||
                    X.cols != (to_col - from_col + 1))
                    {
                        std::cout << from_row << " " << X.cols << "\n";
                        throw MMError("Matrix dimensions do not match.");
                        return *this;
                    }

                T* buffer = new T[(X.rows)*(X.cols)];
                int i, j;
                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        buffer[(i*X.cols)+j] = matrix[from_row+i][from_col+j] - X.get(i,j);

                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        (matrix[from_row+i][from_col+j] = buffer[(i*X.cols)+j]);

                delete[] buffer;
            }

            Slice<T>& operator+ (const Slice<T> & X)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) += X;
                return *A;
            }

            Slice<T>& operator- (const Slice<T> & X)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) -= X;
                return *A;
            }

            Slice<T>& operator* (const Slice<T>& X)
            {
                Slice<T>* A = new Slice<T>(*this);
                (*A) *= X;
                return *A;
            }

            Slice<T>& operator= (Slice<T> &X)
            {
                if (X.rows != (to_row - from_row + 1) ||
                    X.cols != (to_col - from_col + 1))
                    {
                        std::cout << from_row << " " << X.cols << "\n";
                        throw MMError("Matrix dimensions do not match.");
                        return *this;
                    }

                T* buffer = new T[(X.rows)*(X.cols)];
                int i, j;
                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        buffer[(i*X.cols)+j] = X[i][j];

                for (i=0;i<X.rows;i++)
                    for (j=0;j<X.cols;j++)
                        (matrix[from_row+i][from_col+j] = buffer[(i*X.cols)+j]);

                delete[] buffer;
            }

            Slice<T>& slice(int fr, int tr, int fc, int tc)
            {
                // TO DO: Error check
                return *(new Slice<T>(m,fr+from_row,tr+from_row,fc+from_col,tc+from_col));
            }


            T& get(int x, int y)
            {
                return matrix[from_row+x][from_col+x];
            }

    };

};

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
                matrix = m->matrix;
                this->m = m;
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

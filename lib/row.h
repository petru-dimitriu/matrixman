#pragma once

namespace MatrixMan
{
    template <class T>
    class Matrix;

    template <class T>
    class Slice;

    template <class T>
    class RowSlice
    {
        friend class Matrix<T>;

        private:
            T* matrix;
            int from_col, to_col;

        public:
            RowSlice()
            {
                matrix = NULL;
            }

            RowSlice(Matrix<T> *m, int row, int from_col, int to_col)
            {
                this->matrix = m->matrix[row];
                this->from_col = from_col == -1 ? 0 : from_col;
                this->to_col = to_col == -1 ? m->cols-1 : to_col;
            }

            void display()
            {
                for (int i=from_col;i<=to_col;i++)
                    std::cout << matrix[i] << " ";
                std::cout << "\n";
            }

            void fill(T what)
            {
                int i;
                for (i=from_col;i<=to_col;i++)
                    matrix[i] = what;
            }

            inline T& operator[] (int x)
            {
                return get(x);
            }

            RowSlice<T>& operator= (T what)
            {
                fill(what);
                return *this;
            }

            T& get(int x)
            {
                if (x < 0 || x > to_col-from_col+1)
                {
                    throw MMError("Index out of bounds");
                    return matrix[from_col];
                }
                return matrix[from_col+x];
            }

            RowSlice<T>& operator= (RowSlice<T>& X)
            {
                if (to_col - from_col != X.to_col - X.to_col)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] = X.get(x);
                return *this;
            }

            RowSlice<T>& operator+= (RowSlice<T>& X)
            {
                if (to_col - from_col != X.to_col - X.to_col)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] += X.get(x);
                return *this;
            }

            RowSlice<T>& operator-= (RowSlice<T>& X)
            {
                if (to_col - from_col != X.to_col - X.to_col)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] -= X.get(x);
                return *this;
            }

            RowSlice<T>& operator= (Matrix<T>& X)
            {
                if (to_col - from_col +1 != X.cols || X.rows != 1)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] = X.get(1,x);
                return *this;
            }

            RowSlice<T>& operator+= (Matrix<T>& X)
            {
                if (to_col - from_col + 1!= X.cols || X.rows != 1)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] += X.get(1,x);
                return *this;
            }

            RowSlice<T>& operator-= (Matrix<T>& X)
            {
                if (to_col - from_col + 1!= X.cols || X.rows != 1)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] -= X.get(1,x);
                return *this;
            }

            RowSlice<T>& operator= (Slice<T>& X)
            {
                if (to_col - from_col + 1 != X.cols || X.rows != 1)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] = X.get(1,x);
                return *this;
            }

            RowSlice<T>& operator+= (Slice<T>& X)
            {
                if (to_col - from_col + 1!= X.cols || X.rows != 1)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] += X.get(1,x);
                return *this;
            }

            RowSlice<T>& operator-= (Slice<T>& X)
            {
                if (to_col - from_col + 1!= X.cols || X.rows != 1)
                {
                    throw MMError("Row dimensions do not match.");
                    return *this;
                }
                int i, x;
                for (i=from_col, x=0;i<=to_col;i++,x++)
                    matrix[i] -= X.get(1,x);
                return *this;
            }
    };
};

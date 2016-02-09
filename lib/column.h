#pragma once

namespace MatrixMan
{
    template <class T>
    class Matrix;

    template <class T>
    class Slice;

	template <class T>
	class RowSlice;

    template <class T>
    class ColumnSlice
    {
        friend class Matrix<T>;
		friend class RowSlice <T>;

        private:
            T** matrix;
            Matrix<T>* ref;
            int mat_col;
            int from_row, to_row;

        public:
            ColumnSlice()
            {

            }

            ColumnSlice(Matrix<T> *m, int col, int from_row, int to_row)
            {
                ref = m;
                this->mat_col = col;
                this->matrix = m->matrix;
                this->from_row = from_row == -1 ? 0 : from_row;
                this->to_row = to_row == -1 ? m->rows-1 : to_row;
            }

            void display()
            {
                for (int i=from_row;i<=to_row;i++)
                    std::cout << matrix[i][mat_col] << "\n";
            }

            void fill(T what)
            {
                int i;
                for (i=from_row;i<=to_row;i++)
                    matrix[i][mat_col] = what;
            }

            T& operator[] (int x)
            {
                return get(x);
            }

            ColumnSlice<T>& operator= (T what)
            {
                fill(what);
                return *this;
            }

            ColumnSlice<T>& operator= (ColumnSlice<T>& X)
            {
                if (to_row-from_row != X.to_row-X.from_row)
                {
                    throw MMError("Column dimensions do not match.");
                    return *this;
                }
                int x, i;
                for (i=from_row,x=0;i<=to_row;i++,x++)
                    matrix[i][mat_col] = X.get(x);
                return *this;
            }

            ColumnSlice<T>& operator+= (ColumnSlice<T>& X)
            {
                if (to_row-from_row != X.to_row-X.from_row)
                {
                    throw MMError("Column dimensions do not match.");
                    return *this;
                }
                int x, i;
                for (i=from_row,x=0;i<=to_row;i++,x++)
                    matrix[i][mat_col] += X.get(x);
                return *this;
            }

            ColumnSlice<T>& operator-= (ColumnSlice<T>& X)
            {
                if (to_row-from_row != X.to_row-X.from_row)
                {
                    throw MMError("Column dimensions do not match.");
                    return *this;
                }
                int x, i;
                for (i=from_row,x=0;i<=to_row;i++,x++)
                    matrix[i][mat_col] -= X.get(x);
                return *this;
            }

            ColumnSlice<T>& operator= (Matrix<T>& X)
            {
                if (to_row-from_row+1 != X.rows || X.cols != 1)
                {
                    throw MMError("Column and matrix dimensions do not match.");
                    return *this;
                }
                int x, i;
                for (i=from_row,x=0;i<=to_row;i++,x++)
                    matrix[i][mat_col] = X.get(x,0);
                return *this;
            }

            ColumnSlice<T>& operator+= (Matrix<T>& X)
            {
                if (to_row-from_row+1 != X.rows || X.cols != 1)
                {
                    throw MMError("Column and matrix dimensions do not match.");
                    return *this;
                }
                int x, i;
                for (i=from_row,x=0;i<=to_row;i++,x++)
                    matrix[i][mat_col] += X.get(x,0);
                return *this;
            }

            ColumnSlice<T>& operator-= (Matrix<T>& X)
            {
                if (to_row-from_row+1 != X.rows || X.cols != 1)
                {
                    throw MMError("Column and matrix dimensions do not match.");
                    return *this;
                }
                int x, i;
                for (i=from_row,x=0;i<=to_row;i++,x++)
                    matrix[i][mat_col] -= X.get(x,0);
                return *this;
            }

            T& get(int x)
            {
                if (x < 0 || x > to_row-from_row+1)
                {
                    throw MMError("Index out of bounds");
                    return matrix[from_row][mat_col];
                }
                return matrix[from_row+x][mat_col];
            }

			// ---------------------------------------------------------------------------------------

			Matrix<T>& operator*(RowSlice<T>& X)
			{
				if (to_row - from_row != X.to_col - X.from_col)
				{
					throw MMError("Row / Column dimensions do not match.");
					return Matrix<double>();
				}

				Matrix<T>* newMatrix = new Matrix<T>(to_row - from_row + 1, X.to_col - X.from_col + 1, 0);
				for (int i = from_row; i <= to_row; ++i)
					for (int j = X.from_col; j <= X.to_col; ++j)
						newMatrix->matrix[i - from_row][j - X.from_col] = get(i) * X.get(j);
				return *newMatrix;
			}
    };
};


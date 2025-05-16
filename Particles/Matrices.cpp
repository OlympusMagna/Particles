#include "Matrices.h"

namespace Matrices
{
    Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols)
    {
        a.resize(rows);
        for (int i = 0; i < rows; ++i)
        {
            a[i].resize(cols, 0.0); // Initialize all elements to 0.0
        }
    }

    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        // Ensure matrices have the same dimensions for addition.
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {
            throw std::invalid_argument("Matrices must have the same dimensions for addition.");
        }

        Matrix result(a.getRows(), a.getCols());
        for (int i = 0; i < a.getRows(); ++i)
        {
            for (int j = 0; j < a.getCols(); ++j)
            {
                result(i, j) = a(i, j) + b(i, j); // Add corresponding elements.
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& a, const Matrix& b)
    {
        // Ensure the number of columns in the first matrix equals the number of rows in the second.
        if (a.getCols() != b.getRows())
        {
            throw std::invalid_argument("Number of columns in first matrix must equal number of rows in second matrix for multiplication.");
        }

        Matrix result(a.getRows(), b.getCols());
        for (int i = 0; i < a.getRows(); ++i)
        {
            for (int j = 0; j < b.getCols(); ++j)
            {
                for (int k = 0; k < a.getCols(); ++k)
                {
                    result(i, j) += a(i, k) * b(k, j); // Perform matrix multiplication.
                }
            }
        }
        return result;
    }

    bool operator==(const Matrix& a, const Matrix& b)
    {
        // Matrices must have the same dimensions to be equal.
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {
            return false;
        }

        for (int i = 0; i < a.getRows(); ++i)
        {
            for (int j = 0; j < a.getCols(); ++j)
            {
                // If any elements are different, matrices are not equal.
                if (a(i, j) != b(i, j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& a, const Matrix& b)
    {
        // Not equal is the opposite of equal.
        return !(a == b);
    }

    ostream& operator<<(ostream& os, const Matrix& a)
    {
        for (int i = 0; i < a.getRows(); ++i)
        {
            for (int j = 0; j < a.getCols(); ++j)
            {
                os << fixed << setprecision(2) << a(i, j) << " "; // Output with fixed precision.
            }
            os << endl; // Newline after each row.
        }
        return os;
    }

    RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
    {
        // Initialize the 2x2 rotation matrix.
        a[0][0] = cos(theta);
        a[0][1] = -sin(theta);
        a[1][0] = sin(theta);
        a[1][1] = cos(theta);
    }

    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
    {
        // Initialize the 2x2 scaling matrix.
        a[0][0] = scale;
        a[0][1] = 0;
        a[1][0] = 0;
        a[1][1] = scale;
    }

    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
    {
        // Initialize the 2xn translation matrix.
        for (int j = 0; j < nCols; ++j)
        {
            a[0][j] = xShift;
            a[1][j] = yShift;
        }
    }
}

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

namespace Matrices
{
    class Matrix
    {
        public:
            // Construct a matrix with given rows and columns.
            // Initializes all elements to 0.
            Matrix(int _rows, int _cols);

            // Inline accessors/mutators:

            // Read element at (row i, column j)
            // Example: double x = a(i,j);
            const double& operator()(int i, int j) const
            {
                return a.at(i).at(j);
            }

            // Assign element at (row i, column j)
            // Example: a(i,j) = x;
            double& operator()(int i, int j)
            {
                return a.at(i).at(j);
            }

            int getRows() const { return rows; }
            int getCols() const { return cols; }
            // End of inline accessors/mutators
        protected:
            // Changed to protected so subclasses can modify
            vector<vector<double>> a;
        private:
            int rows;
            int cols;
    };

    // Add corresponding elements of two matrices.
    // Example: c = a + b;
    Matrix operator+(const Matrix& a, const Matrix& b);

    // Matrix multiplication.
    // Example: c = a * b;
    Matrix operator*(const Matrix& a, const Matrix& b);

    // Check if two matrices are equal.
    // Example: a == b
    bool operator==(const Matrix& a, const Matrix& b);

    // Check if two matrices are not equal.
    // Example: a != b
    bool operator!=(const Matrix& a, const Matrix& b);

    // Output matrix to stream.
    // Columns separated by spaces, rows by newlines.
    ostream& operator<<(ostream& os, const Matrix& a);

    /*******************************************************************************/

    // 2D rotation matrix.
    // A = R * A rotates A by theta radians counter-clockwise.
    class RotationMatrix : public Matrix
    {
        public:
            // Create a 2x2 rotation matrix.
            // Elements:
            //   cos(theta)  -sin(theta)
            //   sin(theta)   cos(theta)
            // theta: rotation angle in radians (counter-clockwise)
            RotationMatrix(double theta);
    };

    // 2D scaling matrix.
    // A = S * A scales A by a factor.
    class ScalingMatrix : public Matrix
    {
        public:
            // Create a 2x2 scaling matrix.
            // Elements:
            //   scale   0
            //   0       scale
            // scale: scaling factor
            ScalingMatrix(double scale);
    };

    // 2D translation matrix.
    // A = T + A shifts A by (xShift, yShift).
    class TranslationMatrix : public Matrix
    {
        public:
            // Create a 2xn translation matrix.
            // Elements:
            //   xShift  xShift  xShift  ...
            //   yShift  yShift  yShift  ...
            // xShift: horizontal shift, yShift: vertical shift, nCols: number of columns
            // nCols: number of (x, y) coordinate pairs.
            TranslationMatrix(double xShift, double yShift, int nCols);
    };
}

#endif // MATRIX_H_INCLUDED

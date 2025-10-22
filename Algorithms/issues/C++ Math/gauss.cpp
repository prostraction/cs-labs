#include <iomanip>
#include <iostream>


static int iterations = 0;

inline void print(double** matrix)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            std::cout << std::setw(10) << matrix[i][j];
        std::cout  << "    |    "  << matrix[i][3] << std::endl;
    }
    std::cout << std::endl;
}

inline void forwardGaussMove(double **matrix)
{
    for (int row = 0; row < 3; row++)
    {
        double row_row_element = matrix[row][row]; iterations++;
        for (int column = 0; column < 4; column++)
        {
            matrix[row][column] = matrix[row][column] / row_row_element;
            iterations += 2;
        }
        for (int row_after = row + 1; row_after < 3; row_after++)
        {
            double K = matrix[row_after][row] / matrix[row][row]; iterations++;
            for (int column = 0; column < 4; column++)
            {
                matrix[row_after][column] = matrix[row_after][column] - matrix[row][column] * K;
                iterations += 3;
            }
        }
    }
}

inline void backwardGaussMove(double **matrix)
{
    for (int row = 2; row >= 0; row--)
    {
        double row_row_element = matrix[row][row]; iterations++;
        for (int column = 3; column >= 0; column--)
        {
            matrix[row][column] = matrix[row][column] / row_row_element;
            iterations += 2;
        }
        for (int row_after = row - 1; row_after >= 0; row_after--)
        {
            double K = matrix[row_after][row] / matrix[row][row]; iterations++;
            for (int column = 3; column >= 0; column--)
            {
                matrix[row_after][column] = matrix[row_after][column] - matrix[row][column] * K;
                iterations += 3;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);

    const int N = 212;
    double** matrix = new double*[3];
    for (int i = 0; i < 4; i++)
        matrix[i] = new double[4];
    double* right_column = new double[3];
    ////////////////////////////////////////////////////
    matrix[2][0] = 20*N + 15.1;
    matrix[2][1] = 3*N + 4;
    matrix[2][2] = 5*N + 7;
    matrix[2][3] = 30*N + 29.1;
    ////////////////////////////////////////////////////
    matrix[0][0] = 1;
    matrix[0][1] = 10.5 + N;
    matrix[0][2] = N + 0.01;
    matrix[0][3] = 2*N + 1.02;
    ////////////////////////////////////////////////////
    matrix[1][0] = N - 1.9;
    matrix[1][1] = N + 4.002;
    matrix[1][2] = 16 + 4*N;
    matrix[1][3] = 9*N + 30.1;
    ////////////////////////////////////////////////////

    print(matrix);

    /* check for zero does not included */
    const clock_t begin = clock();

    forwardGaussMove(matrix);
    print(matrix);
    backwardGaussMove(matrix);

    const clock_t end = clock();

    print(matrix);
    for (int i = 0; i < 3; i++) right_column[i] = matrix[i][3];
    std::cout << "Roots: \n";
    for (int i = 0; i < 3; i++)
    {
        /* clean out with E = 10^-6 OR use "std::fixed << std::setprecision(2) << right_column[i] << std::endl;" for force precision, but it's not clean */
        std::cout << "x[" << i << "] = " << (abs((right_column[i]) < 0.0000001) ? (0) : (right_column[i])) << std::endl;
    }
    std::cout << "\nIterations: " << iterations;
    std::cout << "\nTime: " << (end-begin)/double(CLOCKS_PER_SEC) << " sec\n";

    for (int i = 0; i < 3; i++)
        delete[] matrix[i];
    delete[] right_column;
    return 0;
}

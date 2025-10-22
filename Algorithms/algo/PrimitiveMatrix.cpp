#include <iostream>

void showMatrix(int rows, int cols, int** matrix) {
    std::cout << "Matrix values:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

int** initializeMatrix(int rows, int cols, int** matrix) {
    std::cout << "Enter matrix values:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cin >> matrix[i][j];
        }
    }
    return matrix;
}

int** transposeMatrix(int rows, int cols, int** matrix) {
    // Создаем новую матрицу с перевернутыми размерами
    int** transposed = new int*[cols];
    for (int i = 0; i < cols; i++) {
        transposed[i] = new int[rows];
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

void deleteMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main() {
    std::cout << "Enter matrix size (rows columns): ";
    int n, m;
    std::cin >> n >> m;

    int** matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[m];
    }

    matrix = initializeMatrix(n, m, matrix);
    std::cout << "\nOriginal matrix:\n";
    showMatrix(n, m, matrix);

    int** transposed = transposeMatrix(n, m, matrix);
    
    std::cout << "\nTransposed matrix:\n";
    showMatrix(m, n, transposed);

    deleteMatrix(matrix, n);
    deleteMatrix(transposed, m);
    
    return 0;
}
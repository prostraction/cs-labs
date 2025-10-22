#include <iostream>

int sumBeaten = 0;
int sumPlaced = 0;

char board[8][8];

bool checker(const int &x, const int &y, const char &fig) {
    if (board[x][y] == '*') {
        sumBeaten++;
        board[x][y] = 'X';
    } else if (board[x][y] != fig && board[x][y] != 'X') {
        return false;
    }
    return true;
}

void checkBoard(const bool &rook, const int &posX, const int &posY) {
    if (rook) {
        for (int j = posY; j < 8; j++) {
            if (!checker(posX, j, 'R')) break;
        }

        for (int j = posY; j >= 0; j--) {
            if (!checker(posX, j, 'R')) break;
        }

        for (int j = posX; j >= 0; j--) {
            if (!checker(j, posY, 'R')) break;
        }

        for (int j = posX; j < 8; j++) {
            if (!checker(j, posY, 'R')) break;
        }
    } else {
        for (int i = posX, j = posY; i < 8 && j < 8; i++, j++) {
            if (!checker(i, j, 'B')) break;
        }

        for (int i = posX, j = posY; i < 8 && j >= 0; i++, j--) {
            if (!checker(i, j, 'B')) break;
        }

        for (int i = posX, j = posY; i >= 0 && j < 8; i--, j++) {
            if (!checker(i, j, 'B')) break;
        }

        for (int i = posX, j = posY; i >= 0 && j >= 0; i--, j--) {
            if (!checker(i, j, 'B')) break;
        }
    }
}

int main() {
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cin >> board[i][j];
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == 'R') {
                sumPlaced++;
                checkBoard(true, i, j);
            } else if (board[i][j] == 'B') {
                sumPlaced++;
                checkBoard(false, i, j);
            }
        }
    }

    /*
    std::cout << std::endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }*/

    std::cout << 64 - sumBeaten - sumPlaced;
}
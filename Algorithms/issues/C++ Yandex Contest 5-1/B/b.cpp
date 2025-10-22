#include <stdio.h>
#include <iostream>

int main() {
    int m1left, m1right;
    int m2left, m2right;
    int home;
    scanf("%d:%d", &m1left, &m1right);
    scanf("%d:%d", &m2left, &m2right);
    scanf("%d", &home);

    int left = m1left + m2left;
    int right = m1right + m2right;
    
    if (left > right) {
        std::cout << 0;
    } else if (left == right) {
        if (m1left > m2right && home == 2) {
            std::cout << 0;
        } else if (m2left > m1right && home == 1) {
            std::cout << 0;
        } else {
            std::cout << 1;
        }
    } else {
        if (m1left > m2right && home == 2) {
            std::cout << abs(right - left);
        } else if (m2left + abs(right - left) > m1right && home == 1) {
            std::cout << abs(right - left);
        } else {
            std::cout << abs(right - left) + 1;
        }
    }

    return 0;
}
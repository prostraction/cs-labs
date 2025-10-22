#include <iostream>

int main() {
    int n;
    std::cin >> n;
    long long a, sum = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> a;
        sum += a / 4;
        if (a % 4 != 0) {
            (a % 4 == 1) ? sum += 1 : sum += 2;
        }
    }
    std::cout << sum;
}
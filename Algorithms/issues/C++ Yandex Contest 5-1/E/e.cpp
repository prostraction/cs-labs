#include <iostream>
#include <vector>

int main() {
    int64_t n, k, d;
    std::cin >> n >> k >> d;

    std::string nEnd;

    bool inter = false;
    bool failed = false;
    for (int i = 0; i < d; i++) {
        if (inter) {
            break;
        }
        bool accept = false;
        for (int j = 0; j < 10; j++) {
            //std::cout << "--: " << n * 10 + j << " " << (n * 10 + j) % k << std::endl;
            if ((n * 10 + j) % k == 0) {
                if (j == 0) {
                    nEnd.insert(0, d-i, '0');
                    accept = true;
                    inter = true;
                    break;
                } else {
                    n = n * 10 + j;
                    accept = true;
                    break;
                }
                
            }
        }
        if (!accept) {
            failed = true;
            std::cout << -1;
            break;
        }
    }

    if (!failed) {
        std::cout << n;
        if (inter) {
            std::cout << nEnd;
        }
    }
}
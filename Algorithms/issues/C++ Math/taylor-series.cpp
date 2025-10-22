#include <iostream>
#include <time.h>
#include <cmath>


int main(int argc, char *argv[])
{
    /* (1 / 1 + a) = 1 - a + a^2 - a^3 + a^4 - a^5 .... */
    int iterations = 6;
    double sum = 1;
    double x = 0.22;
    double a = 0;
    double E = 0.00001;
    double term = 1;
    int sign = -1;

    std::cout << "f(x) = 1 / (1 + 6 *x^4)\n";
    std::cout << "x: "; std::cin >> x;
    if (6 * (x * x * x * x) > 1)
    {std::cout << "[Error] This Taylor Series does not converge at a > 1. The result will be incorrect.\n"; return 0;}

    const clock_t begin = clock();
    a = 6 * (x * x * x * x);
    do
    {
        term = term * a;
        sum += sign * term;
        sign = -sign;
        iterations += 3;
    } while (term > E);

    std::cout << "f(" << x << ") = " << sum << "\n\nIterations: " << iterations << "\n";
    std::cout << 1 / (1 + a) << std::endl;
    return 0;
}

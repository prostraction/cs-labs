#include <iostream>
#include <math.h>



inline double function(double x)
{
    return 4 * x * x * x - 10 * x * x + 20 *x;
    //return sin(x);
}

inline double SimpsonFormula(double x0, double x1, double h)
{
    return (function(x0) + 4 * function(x0 + h/2) + function(x1));
}

int main(int argc, char *argv[])
{
    int iterations = 0;

    double a;
    double b;
    std::cout << "f(x) = integral(4x^4 - 10x^2 + 20x)\na: "; std::cin >> a; std::cout << "b: "; std::cin >> b;
    if (a > b) {double t = a; a = b; b = t; std::cout << "a > b. Now a, b changed to b, a. (a = " << a << ", b = " << b << ")\n";}

    double N = 4;
    std::cout << "N: "; std::cin >> N;

    double h = (b - a) / N;
    double sum = 0;

    double x0 = a;
    double x1 = a + h;

    for (int i = 0; i < N; i++)
    {
        sum += SimpsonFormula(x0, x1, h);
        x0 += h;
        x1 += h;
        iterations += 30;
    }
    std::cout << "Ans: " << (h/6) * sum << std::endl;
    std::cout << "Iterations: " << iterations << ", cycles: " << iterations / 30 << std::endl;
    return 0;
}

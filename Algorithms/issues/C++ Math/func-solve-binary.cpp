#include <iostream>
#include <ctime>

double func(double x, double y)
{
    return (4 * x * x * x * x) + (2 * x * x) + (y);
}

int main(int argc, char *argv[])
{
    double x = 0.0;
    double y = 1;
    int i = 0;
    unsigned int iterations = 0;
    std::cout << "-- dy/dx = 4x^4 + 2x^2 + y;  \n-- x belongs to the segment [0; 1] => x0 = 0, x(last) = 1\n";
    double h; std::cout << "h: "; std::cin >> h;

    int step;
    if (h <= 0.00001) step = 10000;
    else if (h <= 0.0001) step = 1000;
    else if (h <= 0.001) step = 100;
    else if (h <= 0.01) step = 10;
    else    step = 1;

    clock_t begin = clock();
    std::cout << "Eylor's method\n";
    for (; x <= 1.0; x += h)
    {
        if (i%step == 0)
            std::cout << "y[" << i << "]: " << y << std::endl;
        y += h * (func(x, y));
        i++;
        iterations += 9;
    }
    if (step != 1)  std::cout << "y[" << i << "]: " << y << std::endl;
    clock_t end = clock();
    std::cout << "Iterations: " << iterations << ", cycles: " << iterations / 9 << ". Time: " << (end-begin)/double(CLOCKS_PER_SEC) << " sec" << std::endl;
    iterations = 0;
    x = 0;
    y = 1;
    i = 0;

    begin = clock();
    std::cout << "\nAdvanced Eylor's method\n";
    for (; x <= 1.0; x += h)
    {
        if (i%step == 0)
            std::cout << "y[" << i << "]: " << y << std::endl;
        double var1 = y + (h/2) * func(x,y);
        double var2 = func((x + (h/2)), var1);
        double var3 = h * var2;
        y += var3;
        i++;
        iterations += 8 + 3 + 8 + 2 + 1 + 1;
    }
    if (step != 1) std::cout << "y[" << i << "]: " << y << std::endl;
    end = clock();
    std::cout << "Iterations: " << iterations << ", cycles: " << iterations / 23 << ". Time: " << (end-begin)/double(CLOCKS_PER_SEC) << " sec" << std::endl;
    return 0;
}

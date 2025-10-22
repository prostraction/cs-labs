#include <iostream>
#include <vector>

static int iterations = 0;

double F(double x)
{
    //return (5 * x * x * x) - (8 * x * x) - (8 * x) + 5;
    iterations += 8;
    return (5 * x * x * x) - (x * x) - (20 * x) + 4;
}

double F_derivative(double x)
{
    //return (15 * x * x) - (16 * x) - 8;
    iterations += 5;
    return (15 * x * x) - (2 * x) - 20;
}

double solve_F_derivative(bool x1_required)
{
    iterations += 13;
    if (x1_required)
        return (-(-2) + sqrt(((-2) * (-2)) - (4 * 15 * (-20)))) / (2 * (15));
        //return sqrt(((-2) * (-2)) - (4 * 15 * (-20))) / (2 * (-2));
    else
        return (-(-2) - sqrt(((-2) * (-2) - 4 * 15 * (-20)))) / (2 * (15));
        //return ((-2) * (-2)) - (4 * 15 * (-20)) / (2 * (-2));
}

int main(int argc, char *argv[])
{

    std::vector<double> x;
    //x.insert(x.end(), -10.);

    double left_side;
    double right_side;
    double c;


    //x.insert(x.end(), 0.);

    /*
    for (double i = -10.; i < 10.; i += 0.01)
    {
        //std::cout << "F_dir(" << i << ") = " << F_dir(i) << std::endl;

        if (F_dir(i) > 0 && F_dir(i + 0.01) < 0)
        {
            std::cout << "******** Sign changed: from " << (i) << " to " << (i + 0.01) << std::endl;
            x.insert(x.end(), i);
        }
        if (F_dir(i) < 0 && F_dir(i + 0.01) > 0)
        {
            std::cout << "******** Sign changed: from " << i << " to " << i + 0.01 << std::endl;
            x.insert(x.end(), i);

        }

    }
*/
    x.insert(x.end(), solve_F_derivative(false));  iterations++;
    x.insert(x.end(), solve_F_derivative(true));   iterations++;
    x.insert(x.end(), 10.);                 iterations++;
    unsigned int count_of_sign_changes = 0;
    double last_line = -10;
    std::cout << "Line borders: [-10, ";
    for (auto it = x.begin(); it != x.end(); ++it)
    {
        std::cout << *it;
        count_of_sign_changes++;
        if (*it != 10.)
            std::cout << ", ";
    }
    std::cout << "]\n";
    auto it = x.begin();
    //it++;
    left_side = *it;
    right_side = 0;

    while (it != x.end())
    {
        left_side = last_line;
        right_side = *it;
        last_line = right_side;
        iterations += 3;
        std::cout << "Borders: " << left_side << ", " << right_side << "]\n";
        while (true)
        {
            //std::cout << "F(" << double((left_side + right_side) / 2.) << ") = " << F((left_side + right_side) / 2.) << std::endl;
            c = (left_side + right_side) / 2.;
            if (F(left_side) * F(c) < 0)
            {
                right_side = c;
                iterations++;
            }
            else
            {
                left_side = c;
                iterations++;
            }
            iterations++;
            if (F(left_side) * F(c) == 0. || abs(F(left_side) * F(c)) <= 0.00000001)
            {
                //std::cout << "F(" << double((left_side + right_side) / 2.) << ") = " << F((left_side + right_side) / 2.) << std::endl;
                std::cout << "x = " << (left_side + right_side) / 2. << std::endl;
                break;
            }
        }
        it++;
    }
    std::cout << "Iterations: " << iterations << std::endl;
    return 0;
}

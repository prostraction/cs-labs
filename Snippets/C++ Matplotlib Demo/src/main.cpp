#include "Matplotlib/matplotlibcpp.h"
#include "Objects/Circle.h"
#include "Objects/Square.h"
#include "Objects/Triangle.h"
#include "Objects/Rectangle.h"

namespace plt = matplotlibcpp;

int main() {
    Square s1(3);
    s1.set_r(4);

    Square s2(4, 2, 2);

    Circle c(2, 1, 1);
    Triangle t1({0, 0}, {2, 2}, {4, 0}, 1, 1);
    Triangle t2({0., 1.5}, {1.5, 0.});
    Rectangle r(2, 4);
    
    plt::figure_size(700, 700);
    plt::subplot2grid(2, 2, 0, 0);
    plt::plot(s1.getX(),s1.getY(), "");
    plt::plot(s2.getX(),s2.getY(), "");
    plt::title("Квадрат");
    plt::axis("off");

    plt::subplot2grid(2, 2, 0, 1);
    plt::plot(c.getX(), c.getY(), "");
    plt::title("Круг");
    plt::axis("off");

    plt::subplot2grid(2, 2, 1, 0);
    plt::plot(t1.getX(), t1.getY(), "");
    plt::plot(t2.getX(), t2.getY(), "");
    plt::title("Треугольники");
    plt::axis("off");

    plt::subplot2grid(2, 2, 1, 1);
    plt::plot(r.getX(), r.getY(), "");
    plt::xlim(-1, 5);
    plt::ylim(-1, 5);
    plt::title("Прямоугольник");
    plt::axis("off");
    plt::show();
}
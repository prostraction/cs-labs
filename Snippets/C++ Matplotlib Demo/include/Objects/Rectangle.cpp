#include "Rectangle.h"

Rectangle::Rectangle() {
    this->verticesSize = 0;
    this->verticesX.reserve(5);
    this->verticesY.reserve(5);
    this->R = 0;
    this->r = 0;
    this->d = 0;
    this->a = 0;
    this->b = 0;
}

Rectangle::Rectangle(const float &a, 
                        const float &b, 
                        const float &x0, 
                        const float &y0) : a(a), b(b) {
    this->R = 0;
    this->r = 0;
    this->d = 0;
    this->x0 = x0;
    this->y0 = y0;
    this->verticesSize = 5;
    this->verticesX.reserve(5);
    this->recalculateValues();
    this->solve();
}

Rectangle::~Rectangle() {
    this->clear();
}

const void Rectangle::solve() {
    this->clear();
    this->verticesSize = 5;
    this->push({x0, y0});
    this->push({x0, a+y0});
    this->push({b+x0, a+y0});
    this->push({b+x0, y0});
    this->push({x0, y0});
}

const void Rectangle::solve(const float &a, const float &x0, const float &y0) {
    this->a = a;
    this->x0 = x0;
    this->y0 = y0;
    this->verticesSize = 5;
    this->recalculateValues();
    this->solve();
}

const void Rectangle::set_r(const float &r) {
    this->a = r * 2;
    this->recalculateValues();
    this->solve();
}

const void Rectangle::set_d(const float &d) {
    this->a = d / sqrt(2.);
    this->recalculateValues();
    this->solve();
}

const void Rectangle::set_a(const float &a) {
    this->a = a;
    this->recalculateValues();
    this->solve();
}

const void Rectangle::set_x0(const float &x0) {
    this->x0 = x0;
    this->recalculateValues();
    this->solve();
}

const void Rectangle::set_y0(const float &y0) {
    this->y0 = y0;
    this->recalculateValues();
    this->solve();
}

const float Rectangle::get_r() {
    return r;
}

const float Rectangle::get_d() {
    return d;
}

const float Rectangle::get_a() {
    return a;
}

const float Rectangle::get_b() {
    return b;
}

const void Rectangle::recalculateValues() {
    if (this->a > 0 && this->b > 0) {
        this->d = sqrt(pow(this->a, 2) * pow(this->b, 2));
        this->r = (this->a > this->b) ? (this->b / 2.) : (this->a / 2.);
        this->R = sqrt(pow(this->a, 2) * pow(this->b, 2)) / 2.;
    }
}
#include "Square.h"

Square::Square() {
    this->a = 0;
    this->b = 0;
    this->x0 = 0;
    this->y0 = 0;
}

Square::Square(const float &a, const float &x0, const float &y0) {
    this->a = a;
    this->b = a;
    this->x0 = x0;
    this->y0 = y0;
    this->recalculateValues();
    this->solve();
}

Square::~Square() {
    this->clear();
}

const void Square::recalculateValues() {
    this->b = this->a;
    if (this->a > 0) {
        this->d = this->a * sqrt(2.);
        this->r = this->a / 2.;
        this->R = this->a * (sqrt(2.) / 2.);
    }
}
#include "Circle.h"

Circle::Circle() : r(0) {
    this->x0 = x0;
    this->y0 = y0;
    this->verticesSize = 0;
    this->verticesX.reserve(361);
    this->verticesY.reserve(361);
}

Circle::Circle(const float &r, const float &x0, const float &y0) : r(r) {
    this->x0 = x0;
    this->y0 = y0;
    this->verticesX.reserve(361);
    this->verticesY.reserve(361);
    for (auto i = 0; i < 361; i++) {
        this->push({(x0 + std::sin((i % 360) * (M_PI / 180.)) * r), (y0 + std::cos((i % 360) * (M_PI / 180.)) * r)});
    }
    this->verticesSize = 361;
}

Circle::~Circle() {
    this->clear();
}

const void Circle::solve() {
    this->clear();
    for (auto i = 0; i < 361; i++) {
        this->push({(x0 + std::sin((i % 360) * (M_PI / 180.)) * r), (y0 + std::cos((i % 360) * (M_PI / 180.)) * r)});
    }
    this->verticesSize = 361;
}

const void Circle::solve(const float &r, const float &x0, const float &y0) {
    this->r = r;
    this->x0 = x0;
    this->y0 = y0;
    this->solve();
}

const void Circle::set_x0(const float &x0) {
    this->x0 = x0;
    this->solve();
}

const void Circle::set_y0(const float &y0) {
    this->y0 = y0;
    this->solve();
}

const void Circle::set_r(const float &r) {
    this->r = r;
    this->solve();
}

const float Circle::get_r() {
    return r;
}
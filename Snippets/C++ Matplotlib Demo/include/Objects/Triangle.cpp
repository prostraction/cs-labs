#include "Triangle.h"

Triangle::Triangle() {
    this->verticesSize = 0;
}

Triangle::Triangle(const std::pair<float, float>&a, 
                    const std::pair<float, float>&b, 
                    const std::pair<float, float>&c,
                    float x0,
                    float y0) : a(a), b(b), c(c) {
    this->x0 = x0;
    this->y0 = y0;
    this->verticesSize = 4;
    this->solve();
}

Triangle::Triangle(const std::pair<float, float>&b, 
                    const std::pair<float, float>&c,
                    float x0,
                    float y0) : b(b), c(c) {
    this->x0 = x0;
    this->y0 = y0;
    this->verticesSize = 4;
    this->a = {x0, y0};
    this->solve();
}

Triangle::~Triangle() {
    this->clear();
}

const void Triangle::solve() {
    this->clear();
    this->push({a.first + x0, a.second + y0});
    this->push({b.first + x0, b.second + y0});
    this->push({c.first + x0, c.second + y0});
    this->push({a.first + x0, a.second + y0});
    this->verticesSize = 4;
}

const void Triangle::set_a(const std::pair<float, float>&a) {
    this->a.first = a.first;
    this->a.second = a.second;
    this->solve();
}

const void Triangle::set_b(const std::pair<float, float>&b) {
    this->b.first = b.first;
    this->b.second = b.second;
    this->solve();
}

const void Triangle::set_c(const std::pair<float, float>&c) {
    this->c.first = c.first;
    this->c.second = c.second;
    this->solve();
}

const void Triangle::set_x0(const float& x0) {
    this->x0 = x0;
    this->solve();
}

const void Triangle::set_y0(const float& y0) {
    this->y0 = y0;
    this->solve();
}

const std::pair<float, float> Triangle::get_a() {
    return a;
}

const std::pair<float, float> Triangle::get_b() {
    return b;
}

const std::pair<float, float> Triangle::get_c() {
    return c;
}
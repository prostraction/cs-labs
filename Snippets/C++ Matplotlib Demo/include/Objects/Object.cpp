#include "Object.h"

Object::Object() {
    this->verticesSize = 0;
}

Object::Object(const size_t &verticesCount) {
    this->verticesX.reserve(verticesCount);
    this->verticesY.reserve(verticesCount);
    this->verticesSize = verticesCount;
}

Object::~Object() {
    this->clear();
}

const void Object::push(const std::pair<float, float> &p) {
    this->verticesX.push_back(p.first);
    this->verticesY.push_back(p.second);
    this->verticesSize++;
}

const void Object::clear() {
    this->clear();
    this->verticesSize = 0;
}

const void Object::set_x0(const float &x0) {
    this->x0 = x0;
}

const void Object::set_y0(const float &y0) {
    this->y0 = y0;
}

const void Object::set_x(const size_t &ind, const float &x) {
    if (ind < verticesSize) {
        this->verticesX.at(ind) = x;
    }
}

const void Object::set_y(const size_t &ind, const float &y) {
    if (ind < verticesSize) {
        this->verticesY.at(ind) = y;
    }
}

const float Object::get_x0() {
    return x0;
}

const float Object::get_y0() {
    return y0;
}

const std::vector<float> Object::getX() {
    return this->verticesX;
}

const std::vector<float> Object::getY() {
    return this->verticesY;
}
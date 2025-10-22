// Object.h
#ifndef OBJECT_H
#define OBJECT_H

#include <cstddef>
#include <cmath>
#include <vector>
#include <algorithm>

class Object {
    public:
        Object();
        Object(const size_t &verticesCount);
        ~Object();
        const float get_x0();
        const float get_y0();
        const std::vector<float> getX();
        const std::vector<float> getY();
        virtual const void set_x0(const float& x0);
        virtual const void set_y0(const float& y0);
        const void set_x(const size_t &ind, const float &x);
        const void set_y(const size_t &ind, const float &y);
        const void push(const std::pair<float, float> &p);
        const void clear();
    protected:
        std::vector<float> verticesX;
        std::vector<float> verticesY;
        size_t verticesSize;
        float x0;
        float y0;
};
#endif
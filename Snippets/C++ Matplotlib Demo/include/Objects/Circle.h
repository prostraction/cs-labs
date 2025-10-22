// Circle.h
#ifndef CIRCLE_H
#define CIRCLE_H
#include "Object.h"

class Circle : public Object {
    public:
        Circle();
        Circle(const float &r, const float &x0 = 0., const float &y0 = 0.);
        ~Circle();
        const void solve();
        const void solve(const float &r, const float &x0 = 0., const float &y0 = 0.);
        const void set_x0(const float &x0) override;
        const void set_y0(const float &y0) override;
        const void set_r(const float &r);
        const float get_r();
    private:
        float r;
};
#endif
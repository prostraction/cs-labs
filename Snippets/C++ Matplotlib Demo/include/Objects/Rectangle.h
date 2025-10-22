// Rectangle.h
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Object.h"

class Rectangle : public Object {
    public:
        Rectangle();
        Rectangle(const float &a, 
                    const float &b, 
                    const float &x0 = 0., 
                    const float &y0 = 0.);
        ~Rectangle();

        const void solve();
        const void solve(const float &a, 
                            const float &x0 = 0., 
                            const float &y0 = 0.);

        const void set_r(const float &r);
        const void set_d(const float &d);
        const void set_a(const float &a);
        const void set_x0(const float &x0) override;
        const void set_y0(const float &y0) override;

        const float get_r();
        const float get_d();
        const float get_a();
        const float get_b();

    protected:
        const virtual void recalculateValues();

        float a;
        float b;
        float R;
        float r;
        float d;
};
#endif
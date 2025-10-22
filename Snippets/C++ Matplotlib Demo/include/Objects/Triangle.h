// Triangle.h
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"

class Triangle : public Object {
    public:
        Triangle();
        Triangle(const std::pair<float, float>&a, 
                    const std::pair<float, float>&b, 
                    const std::pair<float, float>&c,
                    float x0 = 0.,
                    float y0 = 0.);
        Triangle(const std::pair<float, float>&a, 
                    const std::pair<float, float>&b, 
                    float x0 = 0.,
                    float y0 = 0.);
        ~Triangle();

        const void solve();
        const void set_a(const std::pair<float, float>&a);
        const void set_b(const std::pair<float, float>&b);
        const void set_c(const std::pair<float, float>&c);
        const std::pair<float, float> get_a();
        const std::pair<float, float> get_b();
        const std::pair<float, float> get_c();
        const void set_x0(const float& x0) override;
        const void set_y0(const float& y0) override;
    private:
        std::pair<float, float> a;
        std::pair<float, float> b;
        std::pair<float, float> c;
};
#endif
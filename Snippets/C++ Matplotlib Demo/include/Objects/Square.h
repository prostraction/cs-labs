// Square.h
#ifndef SQUARE_H
#define SQUARE_H

#include "Rectangle.h"

class Square : public Rectangle {
    public:
        Square();
        Square(const float &a, 
                const float &x0 = 0., 
                const float &y0 = 0.);
        ~Square();
    private:
        const void recalculateValues() override;
};
#endif
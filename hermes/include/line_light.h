#ifndef LINE_LIGHT_H
#define LINE_LIGHT_H

#include "light.h"
#include "rgb.h"

// 线光源
class LineLight: public Light
{
private:
    RGB color;
    Vector v;
public:
    LineLight(const RGB& color, const Vector& v): color(color), v(v) {}

    virtual Info light(const Point& p) const
    {
        return (Info){color, v};
    }
};

#endif // LINE_LIGHT_H
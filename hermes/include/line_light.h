#ifndef LINE_LIGHT_H
#define LINE_LIGHT_H

#include "light.h"
#include "rgb.h"
#include "object.h"

// 线光源
class LineLight: public Light
{
private:
    RGB color;
    Vector v;
public:
    // v:光线来的方向
    LineLight(const RGB& color, const Vector& v): color(color), v(Normalize(v)) {}

    virtual Info light(const Point& p) const
    {
        return (Info){color, v};
    }
    virtual bool reachable(const Point& p, const Object* obj) const
    {
        return dcmp(obj->intersect(Ray(p, -v)).t) <= 0;
    }
};

#endif // LINE_LIGHT_H
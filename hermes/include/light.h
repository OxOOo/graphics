#ifndef LIGHT_H
#define LIGHT_H

#include "const.h"
#include "rgb.h"
#include "geometry.h"

class Object;

// 光照
class Light
{
public:
    RGB color;
    Light() {}
    virtual ~Light() {}
    typedef shared_ptr<Light> ptr;
    struct Info
    {
        RGB rgb;
        Vector v2p; // vector from light to p
    };

    // 获取颜色
    virtual Info light(const Point& p) const = 0;

    virtual bool reachable(const Point& p, const Object* obj) const = 0;
};

#endif // LIGHT_H
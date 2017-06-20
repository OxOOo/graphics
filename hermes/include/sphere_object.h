#ifndef SPHERE_OBJECT_H
#define SPHERE_OBJECT_H

#include "object.h"
#include "material.h"

// 球形
class SphereObject: public Object
{
public:
    SphereObject(const Point& center, double radius);

    // 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
    virtual IntersectInfo intersect(const Ray& ray) const;

private:
    Point center;
    double radius;
};

#endif // SPHERE_OBJECT_H
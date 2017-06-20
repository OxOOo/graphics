#include "sphere_object.h"

SphereObject::SphereObject(const Point& center, double radius)
{
    this->center = center;
    this->radius = radius;
}

// 光线碰撞，返回t使得P=ray.s+t*rat.d在图形上
Object::IntersectInfo SphereObject::intersect(const Ray& ray) const
{
    Vector l = center - ray.s;
    if (dcmp(SqrLength(l)-radius*radius) <= 0) return NoIntersect;
    double tp = Dot(l, ray.d);
    double d = SqrLength(l) - tp*tp;
    if (dcmp(d - radius) >= 0) return NoIntersect;
    double t = tp - sqrt(radius*radius - d*d);
    return (IntersectInfo){t, ray.s+ray.d*t, Normalize(ray.s+ray.d*t-center)};
}